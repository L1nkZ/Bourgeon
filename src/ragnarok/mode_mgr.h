#pragma once

#include <memory>

#include "utils/hooking/proxy.h"
#include "yaml-cpp/yaml.h"

class ModeMgr {
 public:
  using Pointer = std::unique_ptr<ModeMgr>;
  enum class ModeType : int32_t { kLogin, kGame };

  ModeMgr(const YAML::Node &modemgr_configuration);
  virtual ~ModeMgr() = default;

  // Hooks
  void SwitchHook(ModeType mode_type, char const *map_name);

 protected:
  static MethodRef<ModeMgr,
                   void (ModeMgr::*)(ModeType mode_type, char const *map_name)>
      SwitchRef;
};
