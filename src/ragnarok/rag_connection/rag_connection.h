#ifndef BOURGEON_RAGNAROK_RAGCONNECTION_H_
#define BOURGEON_RAGNAROK_RAGCONNECTION_H_

#include <memory>

class RagConnection {
 public:
  using Pointer = std::unique_ptr<RagConnection>;

  virtual bool SendPacket(int packet_len, char* packet) = 0;
};

#endif /* BOURGEON_RAGNAROK_RAGCONNECTION_H_ */