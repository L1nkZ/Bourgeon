#ifndef BOURGEON_RAGNAROK_SESSION_H_
#define BOURGEON_RAGNAROK_SESSION_H_

class Session {
 public:
  virtual int GetMaxHp() = 0;
  virtual int GetHp() = 0;
  virtual int GetMaxSp() = 0;
  virtual int GetSp() = 0;
};

#endif  // BOURGEON_RAGNAROK_SESSION_H_