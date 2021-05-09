#pragma once

#include <memory>

#include "utils/hooking/proxy.h"
#include "yaml-cpp/yaml.h"

class LoginMode {
 public:
  using Pointer = std::unique_ptr<LoginMode>;

  LoginMode(const YAML::Node& loginmode_configuration);
  virtual ~LoginMode() = default;

  // Hooks
  void OnUpdateHook();

 protected:
  static MethodRef<LoginMode, void (LoginMode::*)()> LoginMode::OnUpdateRef;
};
