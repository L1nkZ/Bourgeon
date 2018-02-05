#pragma once

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
  virtual std::string GetCharName() const = 0;

  bool GetItemInfoById(int id, ItemInfo &item_info) const;
  std::string GetItemNameById(int id) const;

  // Hooks
  int GetTalkTypeHook(char const *chat_buffer, TalkType *talk_type,
                      void *param);

 protected:
  virtual const std::list<ItemInfo> &item_list() const = 0;

  static MethodRef<Session,
                   int (Session::*)(const char *chatBuf,
                                    enum TalkType *talkType, void *param)>
      GetTalkTypeRef;
};
