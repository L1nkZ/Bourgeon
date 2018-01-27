#ifndef BOURGEON_RAGNAROK_OBJECT_FACTORY_H_
#define BOURGEON_RAGNAROK_OBJECT_FACTORY_H_

#include <memory>
#include "ragconnection/ragconnection.h"
#include "session/session.h"

class ObjectFactory {
 public:
  std::unique_ptr<Session> CreateSession(unsigned long timestamp);
  std::unique_ptr<RagConnection> CreateRagConnection(unsigned long timestamp);
};

#endif BOURGEON_RAGNAROK_OBJECT_FACTORY_H_
