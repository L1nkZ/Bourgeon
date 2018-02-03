#ifndef BOURGEON_RAGNAROK_SESSION_H_
#define BOURGEON_RAGNAROK_SESSION_H_

#include <list>
#include <memory>
#include "item_info.h"
#include "ragnarok/talktype.h"
#include "utils/hooking/proxy.h"

class Session {
 public:
  using Pointer = std::unique_ptr<Session>;

  virtual ~Session() = default;

  virtual uint32_t GetAid() const = 0;
  virtual int GetMaxHp() const = 0;
  virtual int GetHp() const = 0;
  virtual int GetMaxSp() const = 0;
  virtual int GetSp() const = 0;

  ITEM_INFO GetItemInfoById(int nameid) const;

  // Hooks
  int GetTalkTypeHook(char const *chat_buffer, TalkType *talk_type,
                      void *param);

 protected:
  virtual const std::list<struct ITEM_INFO> &item_list() const = 0;

  static MethodRef<Session,
                   int (Session::*)(const char *chatBuf,
                                    enum TalkType *talkType, void *param)>
      GetTalkTypeRef;
};

#endif  // BOURGEON_RAGNAROK_SESSION_H_