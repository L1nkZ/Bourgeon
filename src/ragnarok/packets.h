#ifndef BOURGEON_RAGNAROK_PACKETS_H_
#define BOURGEON_RAGNAROK_PACKETS_H_

enum class PacketHeader { CZ_USE_ITEM = 0x439 };

struct PACKET_CZ_USE_ITEM {
  /* this+0x0 */ short header;
  /* this+0x2 */ unsigned short index;
  /* this+0x4 */ unsigned long aid;
};

#endif /* BOURGEON_RAGNAROK_PACKETS_H_ */
