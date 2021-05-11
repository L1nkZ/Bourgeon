#pragma once

#include <string>

#include "ragnarok/game_mode.h"
#include "ragnarok/login_mode.h"
#include "ragnarok/mode_mgr.h"
#include "ragnarok/rag_connection.h"
#include "ragnarok/session.h"
#include "ragnarok/ui_window_mgr.h"
#include "yaml-cpp/yaml.h"

class RagnarokClient {
 public:
  static const uint32_t kUnknownTimeStamp = 0;

  RagnarokClient();
  ~RagnarokClient();

  bool Initialize();

  uint32_t timestamp() const;
  Session& session() const;
  RagConnection& rag_connection() const;
  UIWindowMgr& window_mgr() const;

  // High level methods implemented by the client
  bool UseItemById(int item_id) const;

 private:
  static YAML::Node LoadConfiguration();
  static uint32_t GetClientTimeStamp();
  static void* GetClientBase();
  static uint32_t ConvertClientTimestamp(uint32_t timestamp);
  static bool SetupImgui();

  uint32_t timestamp_;
  Session::Pointer session_;
  RagConnection::Pointer rag_connection_;
  UIWindowMgr::Pointer window_mgr_;
  ModeMgr::Pointer mode_mgr_;
  LoginMode::Pointer login_mode_;
  GameMode::Pointer game_mode_;
};
