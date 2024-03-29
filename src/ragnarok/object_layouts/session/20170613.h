#pragma once

#include <cstdint>
#include <list>
#include <utility>
#include <vector>

#include "ragnarok/item_info.h"
#include "ragnarok/object_layouts/session/macro_utils.h"

SESSION_IMPLEMENTATION(20170613, {
  /*+0x000*/ int32_t cur_map_type_;
  /*+0x004*/ uint8_t padding0[0x64C];
  /*+0x650*/ char char_name_[0x40];
  /*+0x690*/ uint8_t padding1[0x46C];
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
  /*+0xB3C*/ uint8_t padding2[0x6C4];
  /*+0x1200*/ uint32_t aid_;
  /*+0x1204*/ uint8_t padding3[0xEC];
  /*+0x12F0*/ std::list<ItemInfo> item_list_;
  /*+0x12F8*/ uint8_t padding4[0x3464];
  /*+0x475C*/ TalkTypeTable talk_type_table_;
  /*+0x4768*/ uint8_t padding5[0x3F0];
  /*+0x4B58*/ int32_t hp_;
  /*+0x4B5C*/ int32_t max_hp_;
  /*+0x4B60*/ int32_t sp_;
  /*+0x4B64*/ int32_t max_sp_;
});
