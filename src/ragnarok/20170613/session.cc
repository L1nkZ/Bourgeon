#include "session.h"

#include "utils/hooking/hook_manager.h"
#include "utils/hooking/proxy.h"

#define THIS (reinterpret_cast<Session_20170613::Attributes*>(this_))

Session_20170613::Session_20170613() {
  using namespace hooking;

  // Hooks
  Session::SessionRef = HookManager::Instance().SetHook(
      HookType::kJmpHook, reinterpret_cast<uint8_t*>(0x00A50B70),
      reinterpret_cast<uint8_t*>(void_cast(&Session::SessionHook)));
  Session::GetTalkTypeRef = HookManager::Instance().SetHook(
      HookType::kJmpHook, reinterpret_cast<uint8_t*>(0x00A5E960),
      reinterpret_cast<uint8_t*>(void_cast(&Session::GetTalkTypeHook)));
}

uint32_t Session_20170613::GetAid() const { return THIS->aid_; }

int Session_20170613::GetMaxHp() const { return THIS->max_hp_; }

int Session_20170613::GetHp() const { return THIS->hp_; }

int Session_20170613::GetMaxSp() const { return THIS->max_sp_; }

int Session_20170613::GetSp() const { return THIS->sp_; }

std::string Session_20170613::GetCharName() const {
  static const unsigned char name_key[0x40] = {
      0xB0, 0xA1, 0xB3, 0xAA, 0xB4, 0xD9, 0xB6, 0xF3, 0xB8, 0xB6, 0xB9,
      0xD9, 0xBB, 0xE7, 0xBE, 0xC6, 0xC0, 0xDA, 0xC2, 0xF7, 0xC4, 0xAB,
      0xC5, 0xB8, 0xC6, 0xC4, 0xC7, 0xCF, 0xB0, 0xA1, 0xB3, 0xAA, 0xB4,
      0xD9, 0xB6, 0xF3, 0xB8, 0xB6, 0xB9, 0xD9, 0xBB, 0xE7, 0xBE, 0xC6,
      0xC0, 0xDA, 0xC2, 0xF7, 0xC4, 0xAB, 0xC5, 0xB8, 0xC6, 0xC4, 0xC7,
      0xCF, 0x00, 0x00, 0x00, 0x00, 0xBE, 0xC6, 0xBA, 0xFC};
  char clear_name[0x40];

  memcpy(clear_name, THIS->char_name_, sizeof(clear_name));

  for (size_t i = 0; i < sizeof(clear_name); i++) {
    clear_name[i] ^= name_key[i];
  }

  return std::string(clear_name);
}

const std::list<ItemInfo>& Session_20170613::item_list() const {
  return THIS->item_list_;
}
