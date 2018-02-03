#ifndef BOURGEON_RAGNAROK_UI_WINDOW_MGR_UI_WINDOW_MGR_H_
#define BOURGEON_RAGNAROK_UI_WINDOW_MGR_UI_WINDOW_MGR_H_

#include <memory>
#include "utils/hooking/proxy.h"

enum class UIMessage {
  UIM_CREATESTATUSWND = 0x0,
  UIM_PUSHINTOCHATHISTORY = 0x1,
  UIM_CHANGECHATWNDSTATUS = 0x2,
  UIM_OPENCHATWND = 0x3,
  UIM_LOADINGPERCENT = 0x4,
  UIM_PUSH_INTO_CHATROOM = 0x5,
  UIM_SAVE_CHAT_TO_FILE = 0x6,
  UIM_SAVE_CHAT_TO_FILE_FROM_CHATROOM = 0x7,
  UIM_IME_OPENCANDIDATE = 0x8,
  UIM_IME_CHANGECANDIDATE = 0x9,
  UIM_IME_CLOSECANDIDATE = 0xA,
  UIM_IME_STARTCOMPOSITION = 0xB,
  UIM_IME_COMPSTR = 0xC,
  UIM_IME_ENDCOMPOSITION = 0xD,
  UIM_MAKE_WHISPER_WINDOW = 0xE,
  UIM_CURSOR_CHANGE_ACTION = 0xF,
  UIM_CURSOR_CHANGE_MOTION = 0x10,
  UIM_REPORT_CHAT_TO_FILE = 0x11,
  UIM_REPORT_CHAT_TO_FILE_FROM_CHATROOM = 0x12,
  UIM_CLOSE_GRON_MESSENGER = 0x13,
  UIM_PUSHINTOCHATHISTORY2 = 0x14
};

class UIWindowMgr {
 public:
  using Pointer = std::unique_ptr<UIWindowMgr>;

  virtual ~UIWindowMgr() = default;

  // Original
  bool ProcessPushButton(unsigned long vkey, int new_key, int accurate_key);
  size_t SendMsg(UIMessage message, int val1, int val2, int val3, int val4);

  // Hooks
  bool ProcessPushButtonHook(unsigned long vkey, int new_key, int accurate_key);
  size_t SendMsgHook(UIMessage message, int val1, int val2, int val3, int val4);

 protected:
  static MethodRef<UIWindowMgr,
                   bool (UIWindowMgr::*)(unsigned long vkey, int new_key,
                                         int accurate_key)>
      ProcessPushButtonRef;
  static MethodRef<UIWindowMgr,
                   size_t (UIWindowMgr::*)(int message, int val1, int val2,
                                           int val3, int val4)>
      SendMsgRef;

 protected:
  UIWindowMgr* this_;
};

#endif /* BOURGEON_RAGNAROK_UI_WINDOW_MGR_UI_WINDOW_MGR_H_ */