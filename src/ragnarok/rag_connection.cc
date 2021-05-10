#include "ragnarok/rag_connection.h"

#include <Windows.h>

#include "utils/hooking/hook_manager.h"
#include "utils/log_console.h"

// Pointer to the game's RagConnection singleton instance
std::atomic<RagConnection*> RagConnection::g_ragconnection_ptr(nullptr);

RagConnection::RagConnection(const YAML::Node& ragconnection_configuration) {
  using namespace hooking;

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
  return SendPacketRef(g_ragconnection_ptr.load(), packet_len, packet);
}

void RagConnection::ConnectionHook() {
  LogDebug("RagConnection: 0x{:x}", reinterpret_cast<uintptr_t>(this));
  g_ragconnection_ptr.store(this);
  ConnectionRef(this);
}

bool RagConnection::SendPacketHook(int packet_len, char* packet) {
  return SendPacketRef(this, packet_len, packet);
}

// References
MethodRef<RagConnection, void (RagConnection::*)()>
    RagConnection::ConnectionRef;
MethodRef<RagConnection, bool (RagConnection::*)(int packet_len, char* packet)>
    RagConnection::SendPacketRef;
