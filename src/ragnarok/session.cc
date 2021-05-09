#include "ragnarok/session.h"

#include <iostream>

#include "bourgeon.h"
#include "utils/hooking/hook_manager.h"

Session::Session(const YAML::Node& session_configuration) : this_() {
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

bool Session::GetItemInfoById(int nameid, ItemInfo& item_info) const {
  const std::list<ItemInfo> ilist = item_list();

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

  return std::string(iinfo.item_name_);
}

void Session::SessionHook() {
  LogDebug("Session: " + std::to_string((size_t)this));
  this_ = this;
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
      std::cerr << error.what() << std::endl;
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
