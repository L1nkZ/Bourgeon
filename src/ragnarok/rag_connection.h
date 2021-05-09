#pragma once

#include <memory>

#include "utils/hooking/proxy.h"
#include "yaml-cpp/yaml.h"

class RagConnection {
 public:
  using Pointer = std::unique_ptr<RagConnection>;

  RagConnection(const YAML::Node &ragconnection_configuration);

  virtual ~RagConnection() = default;

  bool SendPacket(int packet_len, char *packet);

  // Hooks
  void ConnectionHook();
  bool SendPacketHook(int packet_len, char *packet);

 protected:
  static MethodRef<RagConnection, void (RagConnection::*)()> ConnectionRef;
  static MethodRef<RagConnection,
                   bool (RagConnection::*)(int packet_len, char *packet)>
      SendPacketRef;

 protected:
  RagConnection *this_;
};
