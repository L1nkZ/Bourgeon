#include "rag_connection.h"

#include <stdint.h>

#include "utils/hooking/hook_manager.h"

RagConnection_20170613::RagConnection_20170613() : RagConnection() {
  using namespace hooking;

  this_ =
      reinterpret_cast<RagConnection*>(0x00FC21D8);  // Client's RagConnection

  // Hooks
  RagConnection::SendPacketRef = HookManager::Instance().SetHook(
      HookType::kJmpHook, reinterpret_cast<uint8_t*>(0x0091E1F0),
      reinterpret_cast<uint8_t*>(void_cast(&RagConnection::SendPacketHook)));
}
