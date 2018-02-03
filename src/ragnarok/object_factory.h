#ifndef BOURGEON_RAGNAROK_OBJECT_FACTORY_H_
#define BOURGEON_RAGNAROK_OBJECT_FACTORY_H_

#include "mode_mgr/mode_mgr.h"
#include "rag_connection/rag_connection.h"
#include "session/session.h"
#include "ui_window_mgr/ui_window_mgr.h"

class ObjectFactory {
 public:
  Session::Pointer CreateSession(unsigned long timestamp);
  RagConnection::Pointer CreateRagConnection(unsigned long timestamp);
  UIWindowMgr::Pointer CreateUIWindowMgr(unsigned long timestamp);
  ModeMgr::Pointer CreateModeMgr(unsigned long timestamp);
};

#endif BOURGEON_RAGNAROK_OBJECT_FACTORY_H_
