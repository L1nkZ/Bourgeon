#include "ui_window_mgr.h"

#include <iostream>

#include "bourgeon.h"

bool UIWindowMgr::ProcessPushButton(unsigned long vkey, int new_key,
                                    int accurate_key) {
  return ProcessPushButtonRef(this_, vkey, new_key, accurate_key);
}

size_t UIWindowMgr::SendMsg(UIMessage message, int val1, int val2, int val3,
                            int val4) {
  return SendMsgRef(this_, static_cast<int>(message), val1, val2, val3, val4);
}

void UIWindowMgr::UIWindowMgrHook() {
  LogDebug("UIWindowMgr: " + std::to_string((size_t)this));
  this_ = this;
  UIWindowMgrRef(this);
}

bool UIWindowMgr::ProcessPushButtonHook(unsigned long vkey, int new_key,
                                        int accurate_key) {
  auto registrees = Bourgeon::Instance().GetCallbackRegistrees("OnKeyDown");

  for (auto registree : registrees) {
    try {
      registree(vkey, new_key, accurate_key);
    } catch (pybind11::error_already_set& error) {
      std::cerr << error.what() << std::endl;
      Bourgeon::Instance().UnregisterCallback("OnKeyDown", registree);
    }
  }

  return ProcessPushButtonRef(this, vkey, new_key, accurate_key);
}

size_t UIWindowMgr::SendMsgHook(UIMessage message, int val1, int val2, int val3,
                                int val4) {
  if (message != UIMessage::UIM_PUSHINTOCHATHISTORY)
    return SendMsgRef(this, static_cast<int>(message), val1, val2, val3, val4);

  auto registrees = Bourgeon::Instance().GetCallbackRegistrees("OnChatMessage");

  const char* chat_buffer = reinterpret_cast<const char*>(val1);
  for (auto registree : registrees) {
    try {
      pybind11::str py_chat = pybind11::reinterpret_steal<pybind11::str>(
          PyUnicode_DecodeLatin1(chat_buffer, strlen(chat_buffer), nullptr));
      registree(py_chat);
    } catch (pybind11::error_already_set& error) {
      std::cerr << error.what() << std::endl;
      Bourgeon::Instance().UnregisterCallback("OnChatMessage", registree);
    }
  }

  return SendMsgRef(this, static_cast<int>(message), val1, val2, val3, val4);
}

// References
MethodRef<UIWindowMgr, void (UIWindowMgr::*)()> UIWindowMgr::UIWindowMgrRef;

MethodRef<UIWindowMgr, bool (UIWindowMgr::*)(unsigned long vkey, int new_key,
                                             int accurate_key)>
    UIWindowMgr::ProcessPushButtonRef;

MethodRef<UIWindowMgr, size_t (UIWindowMgr::*)(int message, int val1, int val2,
                                               int val3, int val4)>
    UIWindowMgr::SendMsgRef;
