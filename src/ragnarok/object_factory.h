#pragma once

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
};
