#pragma once

#include <stdint.h>
#include <list>
#include <vector>

#include "ragnarok/session/session.h"
#include "ragnarok/talktype.h"

class Session_20170613 final : public Session {
 public:
  Session_20170613();

  // void InitTalkTypeTable();
  // int GetBodyState();
  // int GetEffectState();
  // int GetHealthState();
  uint32_t GetAid() const override;
  int GetMaxHp() const override;
  int GetHp() const override;
  int GetMaxSp() const override;
  int GetSp() const override;
  std::string GetCharName() const override;
  // bool IsSiegeMode();
  // bool IsBattleFieldMode();

 protected:
  const std::list<ItemInfo> &item_list() const override;

 private:
  struct Attributes {
    int cur_map_type_;
    uint8_t padding0[0x64C];
    char char_name_[0x40];
    uint8_t padding1[0x46C];
    int mkcount_;
    int haircolor_;
    int deadcount_;
    int head_;
    int weapon_;
    int shield_;
    int body_palette_;
    int head_palette_;
    int accessory_;
    int accessory2_;
    int accessory3_;
    int body_state_;
    int health_state_;
    int effect_state_;
    int pos_x_;
    int pos_y_;
    uint8_t padding2[0x6C4];
    uint32_t aid_;
    uint8_t padding3[0xEC];
    std::list<ItemInfo> item_list_;
    uint8_t padding4[0x3464];
    std::vector<std::pair<char const *, TalkType>> talk_type_table_;
    uint8_t padding5[0x3F0];
    int hp_;
    int max_hp_;
    int sp_;
    int max_sp_;
  };
  Attributes *this_;
};
