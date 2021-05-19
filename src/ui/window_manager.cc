#include "ui/window_manager.h"

namespace ui {

void WindowManager::RegisterWindow(std::shared_ptr<Window> p_window) {
  p_window->Initialize();
  windows_.emplace(std::move(p_window));
}

void WindowManager::UnregisterWindow(std::shared_ptr<Window> p_window) {
  windows_.erase(p_window);
}

void WindowManager::RenderWindows() const {
  for (auto& p_window : windows_) {
    p_window->Show();
  }
}

}  // namespace ui