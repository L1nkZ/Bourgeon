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

  try {
    switch (session_layout.as<uint32_t>()) {
      case 20151102:
        result = std::make_unique<Session_20151102>(session_configuration);
        break;
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
  } catch (std::exception& ex) {
    LogError("CSession configuration is invalid: {}", ex.what());
    return nullptr;
  }
}

RagConnection::Pointer ObjectFactory::CreateRagConnection(
    const YAML::Node& ragconnection_configuration) noexcept {
  try {
    return std::make_unique<RagConnection>(ragconnection_configuration);
  } catch (std::exception& ex) {
    LogError("CRagConnection configuration is invalid: {}", ex.what());
    return nullptr;
  }
}

UIWindowMgr::Pointer ObjectFactory::CreateUIWindowMgr(
    const YAML::Node& uiwindowmgr_configuration) noexcept {
  try {
    return std::make_unique<UIWindowMgr>(uiwindowmgr_configuration);
  } catch (std::exception& ex) {
    LogError(std::string("UIWindowMgr configuration is invalid") + ex.what());
    return nullptr;
  }
}

ModeMgr::Pointer ObjectFactory::CreateModeMgr(
    const YAML::Node& modemgr_configuration) noexcept {
  try {
    return std::make_unique<ModeMgr>(modemgr_configuration);
  } catch (std::exception& ex) {
    LogError("CModeMgr configuration is invalid: {}", ex.what());
    return nullptr;
  }
}

LoginMode::Pointer ObjectFactory::CreateLoginMode(
    const YAML::Node& login_mode_configuration) noexcept {
  try {
    return std::make_unique<LoginMode>(login_mode_configuration);
  } catch (std::exception& ex) {
    LogError("CLoginMode configuration is invalid: {}", +ex.what());
    return nullptr;
  }
}

GameMode::Pointer ObjectFactory::CreateGameMode(
    const YAML::Node& game_mode_configuration) noexcept {
  try {
    return std::make_unique<GameMode>(game_mode_configuration);
  } catch (std::exception& ex) {
    LogError("CGameMode configuration is invalid: {}", ex.what());
    return nullptr;
  }
}