#include "ragnarok/object_factory.h"

#include "ragnarok/object_layouts/session/layouts.h"
#include "utils/log_console.h"

Session::Pointer ObjectFactory::CreateSession(
    const YAML::Node& session_configuration) {
  Session::Pointer result;

  const auto session_layout = session_configuration["layout"];
  if (!session_layout.IsDefined()) {
    LogError("Missing required field 'layout' for Session");
    return nullptr;
  }

  switch (session_layout.as<uint32_t>()) {
    case 20170613:
      result = std::make_unique<Session_20170613>(session_configuration);
      break;
    case 20190116:
      result = std::make_unique<Session_20190116>(session_configuration);
      break;
    default:
      result = nullptr;
      break;
  }

  return result;
}

RagConnection::Pointer ObjectFactory::CreateRagConnection(
    const YAML::Node& ragconnection_configuration) {
  return std::make_unique<RagConnection>(ragconnection_configuration);
}

UIWindowMgr::Pointer ObjectFactory::CreateUIWindowMgr(
    const YAML::Node& uiwindowmgr_configuration) {
  return std::make_unique<UIWindowMgr>(uiwindowmgr_configuration);
}

ModeMgr::Pointer ObjectFactory::CreateModeMgr(
    const YAML::Node& modemgr_configuration) {
  return std::make_unique<ModeMgr>(modemgr_configuration);
}

LoginMode::Pointer ObjectFactory::CreateLoginMode(
    const YAML::Node& login_mode_configuration) {
  return std::make_unique<LoginMode>(login_mode_configuration);
}

GameMode::Pointer ObjectFactory::CreateGameMode(
    const YAML::Node& game_mode_configuration) {
  return std::make_unique<GameMode>(game_mode_configuration);
}