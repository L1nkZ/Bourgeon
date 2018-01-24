#include "session_factory.h"
#include "Session20170613.h"

std::unique_ptr<Session> SessionFactory::Create(unsigned long timestamp) {
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
