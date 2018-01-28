#ifndef BOURGEON_RAGNAROK_OBJECT_FACTORY_H_
#define BOURGEON_RAGNAROK_OBJECT_FACTORY_H_

#include <memory>
#include "ragconnection/ragconnection.h"
#include "session/session.h"
#include "ui_window_mgr/ui_window_mgr.h"

class ObjectFactory {
 public:
  std::unique_ptr<Session> CreateSession(unsigned long timestamp);
  std::unique_ptr<RagConnection> CreateRagConnection(unsigned long timestamp);
  std::unique_ptr<UIWindowMgr> CreateUIWindowMgr(unsigned long timestamp);
};

#endif BOURGEON_RAGNAROK_OBJECT_FACTORY_H_
