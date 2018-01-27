#ifndef BOURGEON_RAGNAROK_SESSION_H_
#define BOURGEON_RAGNAROK_SESSION_H_

#include <list>
#include "item_info.h"

class Session {
 public:
  virtual uint32_t GetAid() const = 0;
  virtual int GetMaxHp() const = 0;
  virtual int GetHp() const = 0;
  virtual int GetMaxSp() const = 0;
  virtual int GetSp() const = 0;

  ITEM_INFO GetItemInfoById(int nameid) const;

 protected:
  virtual const std::list<struct ITEM_INFO>& item_list() const = 0;
};

#endif  // BOURGEON_RAGNAROK_SESSION_H_