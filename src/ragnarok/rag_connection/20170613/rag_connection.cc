#include "rag_connection.h"
#include <stdint.h>

RagConnection_20170613::RagConnection_20170613() {
  rag_connection_ = reinterpret_cast<RagConnection_20170613*>(
      0x00FC21D8);  // Client's RagConnection
}

bool RagConnection_20170613::SendPacket(int packet_len, char* packet) {
  return rag_connection_->SendPacketWrapper(packet_len, packet);
}

// We need this wrapper to make a "clean" thiscall with a correct "this" pointer
// to the original SendPacket method
_declspec(naked) bool RagConnection_20170613::SendPacketWrapper(int packet_len,
                                                                char* packet) {
  static uint32_t send_packet = 0x0091E1F0;
  __asm jmp send_packet;  // Original method address
}
