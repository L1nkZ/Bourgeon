#ifndef BOURGEON_CORE_RAGNAROK_CLIENT_H_
#define BOURGEON_CORE_RAGNAROK_CLIENT_H_

#include <string>
#include "rag_connection/rag_connection.h"
#include "session/session.h"
#include "ui_window_mgr/ui_window_mgr.h"

class RagnarokClient {
 public:
  RagnarokClient();

  bool Initialize();

  unsigned long timestamp() const;
  Session& session() const;
  RagConnection& rag_connection() const;
  UIWindowMgr& window_mgr() const;

  // High level methods implemented by the client
  bool UseItemById(int item_id) const;

 private:
  unsigned long GetClientTimeStamp() const;
  void* GetClientBase() const;
  std::string GetClientFilename() const;
  unsigned long TranslateTimeStamp(const std::string& str) const;

 public:
  const unsigned long kUnknownTimeStamp = 0;

 private:
  unsigned long timestamp_;
  Session::Pointer session_;
  RagConnection::Pointer rag_connection_;
  UIWindowMgr::Pointer window_mgr_;
};

#endif  // BOURGEON_CORE_RAGNAROK_CLIENT_H_