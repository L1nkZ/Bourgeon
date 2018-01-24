#ifndef BOURGEON_RAGNAROK_SESSION20170613_H_
#define BOURGEON_RAGNAROK_SESSION20170613_H_

#include <list>
#include <vector>
#include "item_info.h"
#include "session.h"

class Session20170613 : public Session {
 public:
  Session20170613();

  // void InitTalkTypeTable();
  // int GetBodyState();
  // int GetEffectState();
  // int GetHealthState();
  // uint32_t GetAid();
  int GetMaxHp() override;
  int GetHp() override;
  int GetMaxSp() override;
  int GetSp() override;
  // struct ITEM_INFO* GetItemInfoById(struct ITEM_INFO* itemInfo, int id);
  // bool IsSiegeMode();
  // bool IsBattleFieldMode();

 private:
  struct Attrs {
    int cur_map_type_;
    uint8_t padding0[0xAF8];
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
    uint8_t padding1[0x6C4];
    uint32_t aid_;
    uint8_t padding2[0xEC];
    std::list<struct ITEM_INFO> item_list_;
    uint8_t padding3[0x3464];
    std::vector<std::pair<char const*, enum TALKTYPE>> talk_type_table_;
    uint8_t padding4[0x3F0];
    int hp_;
    int max_hp_;
    int sp_;
    int max_sp_;
  };
  Attrs* attributes_;
};

#endif // BOURGEON_RAGNAROK_SESSION20170613_H_