#include "object_factory.h"
#include "rag_connection/20170613/rag_connection.h"
#include "session/20170613/session.h"
#include "ui_window_mgr/20170613/ui_window_mgr.h"

std::unique_ptr<Session> ObjectFactory::CreateSession(unsigned long timestamp) {
  std::unique_ptr<Session> result;

  switch (timestamp) {
    case 20170613:
      result = std::make_unique<Session_20170613>();
      break;
    default:
      result = nullptr;
  }

  return result;
}

std::unique_ptr<RagConnection> ObjectFactory::CreateRagConnection(
    unsigned long timestamp) {
  std::unique_ptr<RagConnection> result;

  switch (timestamp) {
    case 20170613:
      result = std::make_unique<RagConnection_20170613>();
      break;
    default:
      result = nullptr;
  }

  return result;
}

std::unique_ptr<UIWindowMgr> ObjectFactory::CreateUIWindowMgr(
    unsigned long timestamp) {
  std::unique_ptr<UIWindowMgr> result;

  switch (timestamp) {
    case 20170613:
      result = std::make_unique<UIWindowMgr_20170613>();
      break;
    default:
      result = nullptr;
  }

  return result;
}
