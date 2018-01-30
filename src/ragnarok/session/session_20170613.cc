#include "Session_20170613.h"

Session_20170613::Session_20170613() {
  attributes_ = reinterpret_cast<Session_20170613::Attrs*>(0x00FF6D00);
}

uint32_t Session_20170613::GetAid() const { return attributes_->aid_; }

int Session_20170613::GetMaxHp() const { return attributes_->max_hp_; }

int Session_20170613::GetHp() const { return attributes_->hp_; }

int Session_20170613::GetMaxSp() const { return attributes_->max_sp_; }

int Session_20170613::GetSp() const { return attributes_->sp_; }

const std::list<struct ITEM_INFO>& Session_20170613::item_list() const {
  return attributes_->item_list_;
}
