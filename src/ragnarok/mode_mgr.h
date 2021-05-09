#pragma once

#include <memory>

#include "utils/hooking/proxy.h"
#include "yaml-cpp/yaml.h"

class ModeMgr {
 public:
  using Pointer = std::unique_ptr<ModeMgr>;

  ModeMgr(const YAML::Node &modemgr_configuration);
  virtual ~ModeMgr() = default;

  // Hooks
  void SwitchHook(int mode_type, char const *map_name);

 protected:
  static MethodRef<ModeMgr,
                   void (ModeMgr::*)(int mode_type, char const *map_name)>
      SwitchRef;
};
