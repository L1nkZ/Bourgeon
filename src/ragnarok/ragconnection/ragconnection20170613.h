#ifndef BOURGEON_RAGNAROK_RAGCONNECTION20170613_H_
#define BOURGEON_RAGNAROK_RAGCONNECTION20170613_H_

#include "ragconnection.h"

class RagConnection20170613 final : public RagConnection {
 public:
  RagConnection20170613();

  bool SendPacket(int packet_len, char* packet) override;

 private:
  bool SendPacketWrapper(int packet_len, char* packet);

 private:
  RagConnection20170613* rag_connection_;
};

#endif /* BOURGEON_RAGNAROK_RAGCONNECTION20170613_H_ */