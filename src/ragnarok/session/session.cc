#include "session.h"
#include <iostream>
#include "core/bourgeon.h"

ITEM_INFO Session::GetItemInfoById(int nameid) const {
  auto ilist = item_list();

  for (const auto iinfo : ilist) {
    if (atoi(iinfo.item_name_.c_str()) == nameid) return iinfo;
  }

  return ITEM_INFO();
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

  return GetTalkType(this, chat_buffer, talk_type, param);
}

// References
MethodRef<Session, int (Session::*)(const char* chatBuf,
                                    enum TalkType* talkType, void* param)>
    Session::GetTalkType;
