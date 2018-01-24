#ifndef BOURGEON_RAGNAROK_SESSION_FACTORY_H_
#define BOURGEON_RAGNAROK_SESSION_FACTORY_H_

#include <memory>
#include "session.h"

class SessionFactory {
 public:
  std::unique_ptr<Session> Create(unsigned long timestamp);
};

#endif BOURGEON_RAGNAROK_SESSION_FACTORY_H_
