#include "ragnarok/rag_connection.h"

#include <Windows.h>

#include "utils/hooking/hook_manager.h"
#include "utils/log_console.h"

static CRITICAL_SECTION sendpacket_cs;

RagConnection::RagConnection(const YAML::Node& ragconnection_configuration)
    : this_() {
  using namespace hooking;

  InitializeCriticalSection(&sendpacket_cs);

  // Hooks
  const auto connection_addr = ragconnection_configuration["CConnection"];
  if (!connection_addr.IsDefined()) {
    throw std::exception(
        "Missing required field 'CConnection' for RagConnection");
  }
  RagConnection::ConnectionRef = HookManager::Instance().SetHook(
      HookType::kJmpHook,
      reinterpret_cast<uint8_t*>(connection_addr.as<uint32_t>()),
      reinterpret_cast<uint8_t*>(void_cast(&RagConnection::ConnectionHook)));

  const auto sendpacket_addr = ragconnection_configuration["SendPacket"];
  if (!sendpacket_addr.IsDefined()) {
    throw std::exception(
        "Missing required field 'SendPacket' for RagConnection");
  }
  RagConnection::SendPacketRef = HookManager::Instance().SetHook(
      HookType::kJmpHook,
      reinterpret_cast<uint8_t*>(sendpacket_addr.as<uint32_t>()),
      reinterpret_cast<uint8_t*>(void_cast(&RagConnection::SendPacketHook)));
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
