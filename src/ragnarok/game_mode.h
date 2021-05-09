#pragma once

#include <memory>

#include "utils/hooking/proxy.h"
#include "yaml-cpp/yaml.h"

class GameMode {
 public:
  using Pointer = std::unique_ptr<GameMode>;

  GameMode(const YAML::Node& game_mode_configuration);
  virtual ~GameMode() = default;

  // Hooks
  void OnUpdateHook();

 protected:
  static MethodRef<GameMode, void (GameMode::*)()> GameMode::OnUpdateRef;
};
