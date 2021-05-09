#pragma once

#include "ragnarok/game_mode.h"
#include "ragnarok/login_mode.h"
#include "ragnarok/mode_mgr.h"
#include "ragnarok/rag_connection.h"
#include "ragnarok/session.h"
#include "ragnarok/ui_window_mgr.h"
#include "yaml-cpp/yaml.h"

class ObjectFactory {
 public:
  Session::Pointer CreateSession(const YAML::Node& session_configuration);
  RagConnection::Pointer CreateRagConnection(
      const YAML::Node& ragconnection_configuration);
  UIWindowMgr::Pointer CreateUIWindowMgr(
      const YAML::Node& uiwindowmgr_configuration);
  ModeMgr::Pointer CreateModeMgr(const YAML::Node& modemgr_configuration);
  LoginMode::Pointer CreateLoginMode(
      const YAML::Node& login_mode_configuration);
  GameMode::Pointer CreateGameMode(const YAML::Node& game_mode_configuration);
};
