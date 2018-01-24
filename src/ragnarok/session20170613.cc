#include "Session20170613.h"

Session20170613::Session20170613() {
  attributes_ = reinterpret_cast<Session20170613::Attrs*>(0x00FF6D00);
}

int Session20170613::GetMaxHp() { return attributes_->max_hp_; }

int Session20170613::GetHp() { return attributes_->hp_; }

int Session20170613::GetMaxSp() { return attributes_->max_sp_; }

int Session20170613::GetSp() { return attributes_->sp_; }
