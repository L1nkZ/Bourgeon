#ifndef BOURGEON_RAGNAROK_RAGCONNECTION_H_
#define BOURGEON_RAGNAROK_RAGCONNECTION_H_

class RagConnection {
 public:
  virtual bool SendPacket(int packet_len, char* packet) = 0;
};

#endif /* BOURGEON_RAGNAROK_RAGCONNECTION_H_ */