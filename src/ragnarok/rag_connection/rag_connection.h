#pragma once

#include <memory>

#include "utils/hooking/proxy.h"

class RagConnection {
 public:
  using Pointer = std::unique_ptr<RagConnection>;

  RagConnection();

  virtual ~RagConnection() = default;

  bool SendPacket(int packet_len, char *packet);

  // Hooks
  bool SendPacketHook(int packet_len, char *packet);

 protected:
  static MethodRef<RagConnection,
                   bool (RagConnection::*)(int packet_len, char *packet)>
      SendPacketRef;

 protected:
  RagConnection *this_;
};
