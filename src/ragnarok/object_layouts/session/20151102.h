#pragma once

#include <cstdint>
#include <list>
#include <utility>
#include <vector>

#include "ragnarok/item_info.h"
#include "ragnarok/object_layouts/session/macro_utils.h"

SESSION_IMPLEMENTATION(20151102, {
  /*+0x000*/ int32_t cur_map_type_;
  /*+0x004*/ uint8_t padding0[0x644];
  /*+0x648*/ char char_name_[0x40];
  /*+0x688*/ uint8_t padding1[0x474];
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
  /*+0xB3C*/ uint8_t padding2[0x6C8];
  /*+0x1204*/ uint32_t aid_;
  /*+0x1208*/ uint8_t padding3[0xE0];
  /*+0x12E8*/ std::list<ItemInfo> item_list_;
  /*+0x12F0*/ uint8_t padding4[0x3464];
  /*+0x4754*/ TalkTypeTable talk_type_table_;
  /*+0x4760*/ uint8_t padding5[0x3A4];
  /*+0x4B04*/ int32_t hp_;
  /*+0x4B08*/ int32_t max_hp_;
  /*+0x4B0C*/ int32_t sp_;
  /*+0x4B10*/ int32_t max_sp_;
});
