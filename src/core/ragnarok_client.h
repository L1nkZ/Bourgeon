#ifndef BOURGEON_CORE_RAGNAROK_CLIENT_H_
#define BOURGEON_CORE_RAGNAROK_CLIENT_H_

#include <memory>
#include <string>
#include "ragnarok/ragconnection/ragconnection.h"
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
  bool RegisterHooks();

 public:
  const unsigned long kUnknownTimeStamp = 0;

 private:
  unsigned long timestamp_;
  std::unique_ptr<Session> session_;
  std::unique_ptr<RagConnection> rag_connection_;
  std::unique_ptr<UIWindowMgr> window_mgr_;
};

#endif  // BOURGEON_CORE_RAGNAROK_CLIENT_H_