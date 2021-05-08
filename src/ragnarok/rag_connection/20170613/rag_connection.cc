#include "rag_connection.h"

#include <cstdint>

#include "utils/hooking/hook_manager.h"

RagConnection_20170613::RagConnection_20170613() : RagConnection() {
  using namespace hooking;

  // Hooks
  RagConnection::ConnectionRef = HookManager::Instance().SetHook(
      HookType::kJmpHook, reinterpret_cast<uint8_t*>(0x0091D470),
      reinterpret_cast<uint8_t*>(void_cast(&RagConnection::ConnectionHook)));
  RagConnection::SendPacketRef = HookManager::Instance().SetHook(
      HookType::kJmpHook, reinterpret_cast<uint8_t*>(0x0091E1F0),
      reinterpret_cast<uint8_t*>(void_cast(&RagConnection::SendPacketHook)));
}
