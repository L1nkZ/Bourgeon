#include "ragnarok/session.h"

#include <array>
#include <cstdint>

#include "bourgeon.h"
#include "utils/hooking/hook_manager.h"
#include "utils/log_console.h"

// Pointer to the game's Session singleton instance
std::atomic<Session*> Session::g_session_ptr(nullptr);

Session::Session(const YAML::Node& session_configuration) {
  using namespace hooking;

  // Hooks
  const auto session_addr = session_configuration["CSession"];
  if (!session_addr.IsDefined()) {
    throw std::exception("Missing required field 'CSession' for Session");
  }
  Session::SessionRef = HookManager::Instance().SetHook(
      HookType::kJmpHook,
      reinterpret_cast<uint8_t*>(session_addr.as<uint32_t>()),
      reinterpret_cast<uint8_t*>(void_cast(&Session::SessionHook)));

  const auto getalktype_addr = session_configuration["GetTalkType"];
  if (!getalktype_addr.IsDefined()) {
    throw std::exception("Missing required field 'GetTalkType' for Session");
  }
  Session::GetTalkTypeRef = HookManager::Instance().SetHook(
      HookType::kJmpHook,
      reinterpret_cast<uint8_t*>(getalktype_addr.as<uint32_t>()),
      reinterpret_cast<uint8_t*>(void_cast(&Session::GetTalkTypeHook)));
}

std::string Session::GetCharName() const {
  static const std::array<uint8_t, 0x40> kNameKey = {
      0xB0, 0xA1, 0xB3, 0xAA, 0xB4, 0xD9, 0xB6, 0xF3, 0xB8, 0xB6, 0xB9,
      0xD9, 0xBB, 0xE7, 0xBE, 0xC6, 0xC0, 0xDA, 0xC2, 0xF7, 0xC4, 0xAB,
      0xC5, 0xB8, 0xC6, 0xC4, 0xC7, 0xCF, 0xB0, 0xA1, 0xB3, 0xAA, 0xB4,
      0xD9, 0xB6, 0xF3, 0xB8, 0xB6, 0xB9, 0xD9, 0xBB, 0xE7, 0xBE, 0xC6,
      0xC0, 0xDA, 0xC2, 0xF7, 0xC4, 0xAB, 0xC5, 0xB8, 0xC6, 0xC4, 0xC7,
      0xCF, 0x00, 0x00, 0x00, 0x00, 0xBE, 0xC6, 0xBA, 0xFC};
  std::array<char, 0x40> clear_name;

  memcpy(clear_name.data(), char_name(), clear_name.size());

  for (size_t i = 0; i < clear_name.size(); i++) {
    clear_name[i] ^= kNameKey[i];
  }
  clear_name[clear_name.size() - 1] = '\0';

  return std::string(clear_name.data());
}

bool Session::GetItemInfoById(int nameid, ItemInfo& item_info) const {
  const auto& ilist = item_list();

  for (const auto& iinfo : ilist) {
    if (atoi(iinfo.item_name_.c_str()) == nameid) {
      item_info = iinfo;
      return true;
    }
  }

  return false;
}

std::string Session::GetItemNameById(int id) const {
  ItemInfo iinfo;

  if (!GetItemInfoById(id, iinfo)) {
    return "Unknown item";
  }

  return iinfo.item_name_;
}

void Session::SessionHook() {
  LogDebug("Session: 0x{:x}", reinterpret_cast<uintptr_t>(this));
  g_session_ptr.store(this);
  SessionRef(this);
}

int Session::GetTalkTypeHook(char const* chat_buffer, TalkType* talk_type,
                             void* param) {
  auto registrees = Bourgeon::Instance().GetCallbackRegistrees("OnTalkType");

  for (auto registree : registrees) {
    try {
      pybind11::str py_chat = pybind11::reinterpret_steal<pybind11::str>(
          PyUnicode_DecodeLatin1(chat_buffer, strlen(chat_buffer), nullptr));
      registree(py_chat);
    } catch (pybind11::error_already_set& error) {
      LogError(error.what());
      Bourgeon::Instance().UnregisterCallback("OnTalkType", registree);
    }
  }

  return GetTalkTypeRef(this, chat_buffer, talk_type, param);
}

// References
MethodRef<Session, void (Session::*)()> Session::SessionRef;
MethodRef<Session, int (Session::*)(const char* chatBuf,
                                    enum TalkType* talkType, void* param)>
    Session::GetTalkTypeRef;
