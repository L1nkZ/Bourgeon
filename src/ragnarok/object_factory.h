#pragma once

#include "ragnarok/mode_mgr.h"
#include "ragnarok/rag_connection.h"
#include "ragnarok/session.h"
#include "ragnarok/ui_window_mgr.h"

class ObjectFactory {
 public:
  Session::Pointer CreateSession(unsigned long timestamp);
  RagConnection::Pointer CreateRagConnection(unsigned long timestamp);
  UIWindowMgr::Pointer CreateUIWindowMgr(unsigned long timestamp);
  ModeMgr::Pointer CreateModeMgr(unsigned long timestamp);
};
