#include "ui_window_mgr_20170613.h"
#include <stdint.h>

UIWindowMgr_20170613::UIWindowMgr_20170613() {
  window_mgr_ = reinterpret_cast<UIWindowMgr_20170613*>(0x00E53F00);
}

size_t UIWindowMgr_20170613::SendMsg(int message, int val1, int val2, int val3,
                                     int val4) {
  return window_mgr_->SendMsgWrapper(message, val1, val2, val3, val4);
}

_declspec(naked) size_t UIWindowMgr_20170613::SendMsgWrapper(int message,
                                                             int val1, int val2,
                                                             int val3,
                                                             int val4) {
  static uint32_t send_msg = 0x006BCFC0;
  __asm jmp send_msg;
}
