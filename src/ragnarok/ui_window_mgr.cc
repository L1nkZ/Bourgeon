#include "ui_window_mgr.h"

#include "bourgeon.h"
#include "utils/hooking/hook_manager.h"
#include "utils/log_console.h"

// Pointer to the game's UIWindowMgr singleton instance
std::atomic<UIWindowMgr*> UIWindowMgr::g_uiwindowmgr_ptr(nullptr);

UIWindowMgr::UIWindowMgr(const YAML::Node& uiwindowmgr_configuration) {
  using namespace hooking;

  // Hooks
  const auto uiwindowmgr_addr = uiwindowmgr_configuration["UIWindowMgr"];
  if (!uiwindowmgr_addr.IsDefined()) {
    throw std::exception(
        "Missing required field 'UIWindowMgr' for UIWindowMgr");
  }
  UIWindowMgr::UIWindowMgrRef = HookManager::Instance().SetHook(
      HookType::kJmpHook,
      reinterpret_cast<uint8_t*>(uiwindowmgr_addr.as<uint32_t>()),
      reinterpret_cast<uint8_t*>(void_cast(&UIWindowMgr::UIWindowMgrHook)));

  const auto processpushbtn_addr =
      uiwindowmgr_configuration["ProcessPushButton"];
  if (!processpushbtn_addr.IsDefined()) {
    throw std::exception(
        "Missing required field 'ProcessPushButton' for UIWindowMgr");
  }
  UIWindowMgr::ProcessPushButtonRef = HookManager::Instance().SetHook(
      HookType::kJmpHook,
      reinterpret_cast<uint8_t*>(processpushbtn_addr.as<uint32_t>()),
      reinterpret_cast<uint8_t*>(
          void_cast(&UIWindowMgr::ProcessPushButtonHook)));

  const auto sendmsg_addr = uiwindowmgr_configuration["SendMsg"];
  if (!sendmsg_addr.IsDefined()) {
    throw std::exception("Missing required field 'SendMsg' for UIWindowMgr");
  }
  UIWindowMgr::SendMsgRef = HookManager::Instance().SetHook(
      HookType::kJmpHook,
      reinterpret_cast<uint8_t*>(sendmsg_addr.as<uint32_t>()),
      reinterpret_cast<uint8_t*>(void_cast(&UIWindowMgr::SendMsgHook)));
}

bool UIWindowMgr::ProcessPushButton(unsigned long vkey, int new_key,
                                    int accurate_key) {
  return ProcessPushButtonRef(g_uiwindowmgr_ptr.load(), vkey, new_key,
                              accurate_key);
}

size_t UIWindowMgr::SendMsg(UIMessage message, int val1, int val2, int val3,
                            int val4) {
  return SendMsgRef(g_uiwindowmgr_ptr.load(), static_cast<int>(message), val1,
                    val2, val3, val4);
}

void UIWindowMgr::UIWindowMgrHook() {
  LogDebug("UIWindowMgr: 0x{:x}", reinterpret_cast<uintptr_t>(this));
  g_uiwindowmgr_ptr.store(this);
  UIWindowMgrRef(this);
}

bool UIWindowMgr::ProcessPushButtonHook(unsigned long vkey, int new_key,
                                        int accurate_key) {
  auto registrees = Bourgeon::Instance().GetCallbackRegistrees("OnKeyDown");

  for (auto registree : registrees) {
    try {
      registree(vkey, new_key, accurate_key);
    } catch (pybind11::error_already_set& error) {
      LogError(error.what());
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
      LogError(error.what());
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
