#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "ui/window.h"

namespace ui {

class WindowManager {
 public:
  void RegisterWindow(std::shared_ptr<Window> p_window);
  void UnregisterWindow(std::shared_ptr<Window> p_window);
  void RenderWindows() const;

 private:
  std::unordered_set<std::shared_ptr<Window>> windows_;
};

}  // namespace ui