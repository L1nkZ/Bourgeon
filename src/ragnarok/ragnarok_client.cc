#include "ragnarok/ragnarok_client.h"

#include <Windows.h>

#include <array>
#include <iomanip>
#include <sstream>

#include "backends/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx7.h"
#include "ragnarok/configuration.h"
#include "ragnarok/object_factory.h"
#include "ragnarok/packets.h"
#include "utils/byte_pattern.h"
#include "utils/hooking/hook_manager.h"
#include "utils/log_console.h"

using CreateWindowExAFunc = HWND(WINAPI*)(DWORD, LPCSTR, LPCSTR, DWORD, int,
                                          int, int, int, HWND, HMENU, HINSTANCE,
                                          LPVOID);
using WindowProcFunc = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg,
                                                      WPARAM wParam,
                                                      LPARAM lParam);
static HWND WINAPI CreateWindowExAHook(DWORD, LPCSTR, LPCSTR, DWORD, int, int,
                                       int, int, HWND, HMENU, HINSTANCE,
                                       LPVOID);
static LRESULT CALLBACK WindowProcHook(HWND hwnd, UINT uMsg, WPARAM wParam,
                                       LPARAM lParam);

static CreateWindowExAFunc CreateWindowExARef;
static WindowProcFunc WndProcRef;

RagnarokClient::RagnarokClient()
    : timestamp_(),
      session_(),
      rag_connection_(),
      window_mgr_(),
      login_mode_(),
      game_mode_() {}

RagnarokClient::~RagnarokClient() { ImGui_ImplWin32_Shutdown(); }

bool RagnarokClient::Initialize() {
  timestamp_ = GetClientTimeStamp();
  if (timestamp_ == kUnknownTimeStamp) {
    LogError("Failed to determine client date");
    return false;
  }
  const auto timestamp_as_str = std::to_string(timestamp_);
  LogInfo("Detected client: {}", timestamp_as_str);

  const YAML::Node configuration = LoadConfiguration();
  const auto client_configuration = configuration[timestamp_as_str];
  if (!client_configuration.IsDefined()) {
    LogError("This client isn't supported");
    return false;
  }

  ObjectFactory factory;
  session_ = factory.CreateSession(client_configuration["CSession"]);
  if (!session_) {
    return false;
  }

  rag_connection_ =
      factory.CreateRagConnection(client_configuration["CRagConnection"]);
  if (!rag_connection_) {
    return false;
  }

  window_mgr_ = factory.CreateUIWindowMgr(client_configuration["UIWindowMgr"]);
  if (!window_mgr_) {
    return false;
  }

  mode_mgr_ = factory.CreateModeMgr(client_configuration["CModeMgr"]);
  if (!mode_mgr_) {
    return false;
  }

  login_mode_ = factory.CreateLoginMode(client_configuration["CLoginMode"]);
  if (!login_mode_) {
    return false;
  }

  game_mode_ = factory.CreateGameMode(client_configuration["CGameMode"]);
  if (!game_mode_) {
    return false;
  }

  // Setup hooks required for imgui's IO
  SetupImgui();

  return true;
}

YAML::Node RagnarokClient::LoadConfiguration() {
  return YAML::Load(kYamlConfiguration);
}

uint32_t RagnarokClient::timestamp() const { return timestamp_; }

Session& RagnarokClient::session() const { return *session_; }

RagConnection& RagnarokClient::rag_connection() const {
  return *rag_connection_;
}

UIWindowMgr& RagnarokClient::window_mgr() const { return *window_mgr_; }

bool RagnarokClient::UseItemById(int item_id) const {
  PACKET_CZ_USE_ITEM packet;
  ItemInfo iinfo;

  if (!session_->GetItemInfoById(item_id, iinfo)) {
    return false;
  }

  packet.header = static_cast<int16_t>(PacketHeader::CZ_USE_ITEM);
  packet.index = static_cast<uint16_t>(iinfo.item_index_);
  packet.aid = session_->aid();

  return rag_connection_->SendPacket(sizeof(packet),
                                     reinterpret_cast<char*>(&packet));
}

uint32_t RagnarokClient::GetClientTimeStamp() {
  const auto* const p_client_base =
      static_cast<const uint8_t*>(GetClientBase());
  if (p_client_base == nullptr) {
    return RagnarokClient::kUnknownTimeStamp;
  }

  const auto* p_dos_header =
      reinterpret_cast<const IMAGE_DOS_HEADER*>(p_client_base);
  const auto* p_nt_headers = reinterpret_cast<const IMAGE_NT_HEADERS*>(
      p_client_base + p_dos_header->e_lfanew);

  // Check PE timestamp
  if (p_nt_headers->FileHeader.TimeDateStamp != 0) {
    return ConvertClientTimestamp(p_nt_headers->FileHeader.TimeDateStamp);
  }

  const IMAGE_DATA_DIRECTORY& dir =
      p_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
  if (dir.Size == 0 || dir.VirtualAddress == 0) {
    return kUnknownTimeStamp;
  }

  // Check the debug data directory timestamp
  const auto* p_dbg_dir = reinterpret_cast<const IMAGE_DEBUG_DIRECTORY*>(
      p_client_base + dir.VirtualAddress);
  if (p_dbg_dir->TimeDateStamp != 0) {
    return ConvertClientTimestamp(p_dbg_dir->TimeDateStamp);
  }

  return kUnknownTimeStamp;
}

void* RagnarokClient::GetClientBase() {
  return static_cast<void*>(GetModuleHandleW(nullptr));
}

uint32_t RagnarokClient::ConvertClientTimestamp(uint32_t timestamp) {
  const std::time_t temp = timestamp;
  std::tm time{};
  gmtime_s(&time, &temp);
  return (time.tm_year + 1900) * 10000 + (time.tm_mon + 1) * 100 + time.tm_mday;
}

bool RagnarokClient::SetupImgui() {
  using namespace hooking;

  const HMODULE h_user32 = GetModuleHandleA("user32.dll");
  if (h_user32 == nullptr) {
    LogError("Failed to get user32.dll's handle");
    return false;
  }

  auto* api_addr =
      reinterpret_cast<uint8_t*>(GetProcAddress(h_user32, "CreateWindowExA"));
  if (api_addr == nullptr) {
    LogError("Failed to resolve CreateWindowExA's address");
    return false;
  }

  CreateWindowExARef =
      reinterpret_cast<CreateWindowExAFunc>(HookManager::Instance().SetHook(
          HookType::kJmpHook, api_addr,
          reinterpret_cast<uint8_t*>(CreateWindowExAHook)));
  return CreateWindowExARef != nullptr;
}

static HWND WINAPI CreateWindowExAHook(DWORD dwExStyle, LPCSTR lpClassName,
                                       LPCSTR lpWindowName, DWORD dwStyle,
                                       int X, int Y, int nWidth, int nHeight,
                                       HWND hWndParent, HMENU hMenu,
                                       HINSTANCE hInstance, LPVOID lpParam) {
  using namespace hooking;

  const auto hwnd = CreateWindowExARef(dwExStyle, lpClassName, lpWindowName,
                                       dwStyle, X, Y, nWidth, nHeight,
                                       hWndParent, hMenu, hInstance, lpParam);
  if (hwnd == nullptr) {
    return hwnd;
  }

  // Hook WndProc
  WNDCLASSEXA wnd_class;
  wnd_class.cbSize = sizeof(wnd_class);
  if (!GetClassInfoExA(hInstance, lpClassName, &wnd_class)) {
    return hwnd;
  }
  if (wnd_class.lpfnWndProc == nullptr) {
    LogError("WndProc was nullptr, cannot hook");
    return hwnd;
  }

  WndProcRef = reinterpret_cast<WindowProcFunc>(HookManager::Instance().SetHook(
      HookType::kJmpHook, reinterpret_cast<uint8_t*>(wnd_class.lpfnWndProc),
      reinterpret_cast<uint8_t*>(WindowProcHook)));
  LogDebug("Hooked WndProc: 0x{:x}",
           reinterpret_cast<size_t>(wnd_class.lpfnWndProc));

  // Start initializing imgui
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplWin32_Init(hwnd);
  ImGuiIO& io = ImGui::GetIO();
  io.MouseDrawCursor = true;

  return hwnd;
}

static LRESULT CALLBACK WindowProcHook(HWND hwnd, UINT uMsg, WPARAM wParam,
                                       LPARAM lParam) {
  const LRESULT result =
      ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);

  ImGuiIO& io = ImGui::GetIO();
  // "Capture" mouse/keyboard inputs when imgui uses them
  if ((uMsg > WM_MOUSEFIRST && uMsg < WM_MOUSELAST && io.WantCaptureMouse) ||
      (uMsg > WM_KEYFIRST && uMsg < WM_KEYLAST &&
       (io.WantCaptureKeyboard || io.WantTextInput))) {
    return result;
  }

  return WndProcRef(hwnd, uMsg, wParam, lParam);
}