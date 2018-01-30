#ifndef BOURGEON_RAGNAROK_UI_WINDOW_MGR_UI_WINDOW_MGR20170613_H_
#define BOURGEON_RAGNAROK_UI_WINDOW_MGR_UI_WINDOW_MGR20170613_H_

#include "ui_window_mgr.h"

class UIWindowMgr_20170613 final : public UIWindowMgr {
 public:
  UIWindowMgr_20170613();

  size_t SendMsg(int message, int val1, int val2, int val3, int val4) override;

 private:
  size_t SendMsgWrapper(int message, int val1, int val2, int val3, int val4);

 private:
  UIWindowMgr_20170613* window_mgr_;
};

#endif /* BOURGEON_RAGNAROK_UI_WINDOW_MGR_UI_WINDOW_MGR20170613_H_ */