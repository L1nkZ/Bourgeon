#include "ragnarok/object_factory.h"
#include "ragnarok/mode_mgr/20170613/mode_mgr.h"
#include "ragnarok/rag_connection/20170613/rag_connection.h"
#include "ragnarok/session/20170613/session.h"
#include "ragnarok/ui_window_mgr/20170613/ui_window_mgr.h"

Session::Pointer ObjectFactory::CreateSession(unsigned long timestamp) {
  Session::Pointer result;

  switch (timestamp) {
    case 20170613:
      result = std::make_unique<Session_20170613>();
      break;
    default:
      result = nullptr;
  }

  return result;
}

RagConnection::Pointer ObjectFactory::CreateRagConnection(
    unsigned long timestamp) {
  RagConnection::Pointer result;

  switch (timestamp) {
    case 20170613:
      result = std::make_unique<RagConnection_20170613>();
      break;
    default:
      result = nullptr;
  }

  return result;
}

UIWindowMgr::Pointer ObjectFactory::CreateUIWindowMgr(unsigned long timestamp) {
  UIWindowMgr::Pointer result;

  switch (timestamp) {
    case 20170613:
      result = std::make_unique<UIWindowMgr_20170613>();
      break;
    default:
      result = nullptr;
  }

  return result;
}

ModeMgr::Pointer ObjectFactory::CreateModeMgr(unsigned long timestamp) {
  ModeMgr::Pointer result;

  switch (timestamp) {
    case 20170613:
      result = std::make_unique<ModeMgr_20170613>();
      break;
    default:
      result = nullptr;
  }

  return result;
}
