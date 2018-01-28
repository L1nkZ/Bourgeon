#ifndef BOURGEON_RAGNAROK_UI_WINDOW_MGR_UI_WINDOW_MGR_H_
#define BOURGEON_RAGNAROK_UI_WINDOW_MGR_UI_WINDOW_MGR_H_

class UIWindowMgr {
 public:
  virtual size_t SendMsg(int message, int val1, int val2, int val3,
                         int val4) = 0;
};

#endif /* BOURGEON_RAGNAROK_UI_WINDOW_MGR_UI_WINDOW_MGR_H_ */