#pragma once

#include <string>

struct ItemInfo {
  int item_type_;
  int location_;
  unsigned long item_index_;
  int wear_location_;
  int num_;
  int price_;
  int real_price_;
  int slot_[0x4];
  std::string item_name_;
  unsigned char is_identified_;
  unsigned char is_damaged_;
  int refining_level_;
  unsigned short is_yours_;
  long delete_time_;
};
