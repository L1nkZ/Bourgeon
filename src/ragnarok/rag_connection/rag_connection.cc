#include "ragnarok/rag_connection/rag_connection.h"

#include <Windows.h>

#include "utils/log_console.h"

static CRITICAL_SECTION sendpacket_cs;

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

void RagConnection::ConnectionHook() {
  LogDebug("RagConnection: " + std::to_string((size_t)this));
  this_ = this;
  ConnectionRef(this);
}

bool RagConnection::SendPacketHook(int packet_len, char* packet) {
  bool result;

  EnterCriticalSection(&sendpacket_cs);
  result = SendPacketRef(this, packet_len, packet);
  LeaveCriticalSection(&sendpacket_cs);

  return result;
}

// References
MethodRef<RagConnection, void (RagConnection::*)()>
    RagConnection::ConnectionRef;
MethodRef<RagConnection, bool (RagConnection::*)(int packet_len, char* packet)>
    RagConnection::SendPacketRef;
