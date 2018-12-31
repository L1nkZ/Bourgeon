#pragma once

#include <string>

#include "ragnarok/mode_mgr/mode_mgr.h"
#include "ragnarok/rag_connection/rag_connection.h"
#include "ragnarok/session/session.h"
#include "ragnarok/ui_window_mgr/ui_window_mgr.h"

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
  ModeMgr::Pointer mode_mgr_;
};
