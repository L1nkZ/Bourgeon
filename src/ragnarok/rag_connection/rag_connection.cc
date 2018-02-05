#include "rag_connection.h"

#include <Windows.h>

CRITICAL_SECTION sendpacket_cs;

RagConnection::RagConnection() : this_() {
  InitializeCriticalSection(&sendpacket_cs);
}

bool RagConnection::SendPacket(int packet_len, char* packet) {
  bool result;

  EnterCriticalSection(&sendpacket_cs);
  result = SendPacketRef(this_, packet_len, packet);
  LeaveCriticalSection(&sendpacket_cs);

  return result;
}

bool RagConnection::SendPacketHook(int packet_len, char* packet) {
  bool result;

  EnterCriticalSection(&sendpacket_cs);
  result = SendPacketRef(this, packet_len, packet);
  LeaveCriticalSection(&sendpacket_cs);

  return result;
}

// References
MethodRef<RagConnection, bool (RagConnection::*)(int packet_len, char* packet)>
    RagConnection::SendPacketRef;
