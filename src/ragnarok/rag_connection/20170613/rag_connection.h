#ifndef BOURGEON_RAGNAROK_RAGCONNECTION_20170613_RAGCONNECTION_H_
#define BOURGEON_RAGNAROK_RAGCONNECTION_20170613_RAGCONNECTION_H_

#include "ragnarok/rag_connection/rag_connection.h"

class RagConnection_20170613 final : public RagConnection {
 public:
  RagConnection_20170613();

  bool SendPacket(int packet_len, char* packet) override;

 private:
  bool SendPacketWrapper(int packet_len, char* packet);

 private:
  RagConnection_20170613* rag_connection_;
};

#endif /* BOURGEON_RAGNAROK_RAGCONNECTION_20170613_RAGCONNECTION_H_ */