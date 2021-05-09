#include "ragnarok/ragnarok_client.h"

#include <Windows.h>

#include <array>
#include <iomanip>
#include <sstream>

#include "ragnarok/configuration.h"
#include "ragnarok/object_factory.h"
#include "ragnarok/packets.h"
#include "utils/byte_pattern.h"
#include "utils/hooking/hook_manager.h"
#include "utils/log_console.h"

RagnarokClient::RagnarokClient()
    : timestamp_(), session_(), rag_connection_(), window_mgr_() {}

bool RagnarokClient::Initialize() {
  timestamp_ = GetClientTimeStamp();
  if (timestamp_ == kUnknownTimeStamp) {
    LogError("Failed to determine client date");
    return false;
  }
  const auto timestamp_as_str = std::to_string(timestamp_);
  LogInfo("Detected client: " + timestamp_as_str);

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

  packet.header = static_cast<short>(PacketHeader::CZ_USE_ITEM);
  packet.index = (unsigned short)iinfo.item_index_;
  packet.aid = session_->aid();

  return rag_connection_->SendPacket(sizeof(PACKET_CZ_USE_ITEM),
                                     (char*)&packet);
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

  const std::time_t temp = p_nt_headers->FileHeader.TimeDateStamp;
  std::tm time{};
  gmtime_s(&time, &temp);

  return (time.tm_year + 1900) * 10000 + (time.tm_mon + 1) * 100 + time.tm_mday;
}

void* RagnarokClient::GetClientBase() {
  return static_cast<void*>(GetModuleHandleW(nullptr));
}

std::string RagnarokClient::GetClientFilename() {
  std::array<char, MAX_PATH> filename;

  GetModuleFileNameA(nullptr, filename.data(), filename.size());
  return std::string(filename.data());
}
