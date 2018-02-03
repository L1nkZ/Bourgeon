#include "session.h"
#include "utils/hooking/hook_manager.h"
#include "utils/hooking/proxy.h"

Session_20170613::Session_20170613() {
  using namespace hooking;

  // Object address
  attributes_ = reinterpret_cast<Session_20170613::Attrs*>(0x00FF6D00);

  // Hooks
  Session::GetTalkTypeRef = HookManager::Instance().SetHook(
      HookType::kJmpHook, reinterpret_cast<uint8_t*>(0x00A5E960),
      reinterpret_cast<uint8_t*>(void_cast(&Session::GetTalkTypeHook)));
}

uint32_t Session_20170613::GetAid() const { return attributes_->aid_; }

int Session_20170613::GetMaxHp() const { return attributes_->max_hp_; }

int Session_20170613::GetHp() const { return attributes_->hp_; }

int Session_20170613::GetMaxSp() const { return attributes_->max_sp_; }

int Session_20170613::GetSp() const { return attributes_->sp_; }

const std::list<struct ITEM_INFO>& Session_20170613::item_list() const {
  return attributes_->item_list_;
}
