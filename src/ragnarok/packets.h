#pragma once

enum class PacketHeader { CZ_USE_ITEM = 0x439 };

#pragma pack(push, 1)

struct PACKET_CZ_USE_ITEM {
  int16_t header;
  uint16_t index;
  uint32_t aid;
};

#pragma pack(pop)
