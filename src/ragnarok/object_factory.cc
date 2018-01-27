#include "object_factory.h"
#include "ragconnection/ragconnection20170613.h"
#include "session/session20170613.h"

std::unique_ptr<Session> ObjectFactory::CreateSession(unsigned long timestamp) {
  std::unique_ptr<Session> result;

  switch (timestamp) {
    case 20170613:
      result = std::make_unique<Session20170613>();
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
      result = std::make_unique<RagConnection20170613>();
      break;
    default:
      result = nullptr;
  }

  return result;
}
