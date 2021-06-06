#pragma once

#include <cstdint>
#include <list>
#include <utility>
#include <vector>

#include "ragnarok/item_info.h"
#include "ragnarok/object_layouts/session/macro_utils.h"

SESSION_IMPLEMENTATION(20190116, {
  /*+0x000*/ int32_t cur_map_type_;
  /*+0x004*/ uint8_t padding0[0x640];
  /*+0x644*/ char char_name_[0x40];
  /*+0x684*/ uint8_t padding1[0x478];
  /*+0xAFC*/ int32_t mkcount_;
  /*+0xB00*/ int32_t haircolor_;
  /*+0xB04*/ int32_t deadcount_;
  /*+0xB08*/ int32_t head_;
  /*+0xB0C*/ int32_t weapon_;
  /*+0xB10*/ int32_t shield_;
  /*+0xB14*/ int32_t body_palette_;
  /*+0xB18*/ int32_t head_palette_;
  /*+0xB1C*/ int32_t accessory_;
  /*+0xB20*/ int32_t accessory2_;
  /*+0xB24*/ int32_t accessory3_;
  /*+0xB28*/ int32_t body_state_;
  /*+0xB2C*/ int32_t health_state_;
  /*+0xB30*/ int32_t effect_state_;
  /*+0xB34*/ int32_t pos_x_;
  /*+0xB38*/ int32_t pos_y_;
  /*+0xB3C*/ uint8_t padding2[0x948];
  /*+0x1484*/ uint32_t aid_;
  /*+0x1488*/ uint8_t padding3[0xF0];
  /*+0x1578*/ std::list<ItemInfo> item_list_;
  /*+0x1580*/ uint8_t padding4[0x3554];
  /*+0x4AD4*/ TalkTypeTable talk_type_table_;
  /*+0x4AE0*/ uint8_t padding5[0x3A0];
  /*+0x4E80*/ int32_t hp_;
  /*+0x4E84*/ int32_t max_hp_;
  /*+0x4E88*/ int32_t sp_;
  /*+0x4E8C*/ int32_t max_sp_;
});
