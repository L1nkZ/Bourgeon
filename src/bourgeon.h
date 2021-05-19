#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "pybind11/embed.h"
#include "ragnarok/ragnarok_client.h"
#include "ui/window_manager.h"

class Bourgeon {
 public:
  // Singleton stuff
  static Bourgeon& Instance() {
    static Bourgeon instance;
    return instance;
  }
  Bourgeon(Bourgeon const&) = delete;
  void operator=(Bourgeon const&) = delete;

  RagnarokClient& client();
  ui::WindowManager& window_manager();

  bool Initialize();
  void OnTick();
  void AddLogLine(std::string log_line);
  void RenderUI() const;

  // Python related
  void RegisterCallback(const std::string& callback_name,
                        const pybind11::object& function);
  void UnregisterCallback(const std::string& callback_name,
                          const pybind11::object& function);
  const std::vector<pybind11::object>& GetCallbackRegistrees(
      const std::string& callback_name);

 private:
  Bourgeon();

  void LoadPlugins(const std::string& folder);
  void ShowBourgeonWindow() const;

  pybind11::scoped_interpreter interpreter_;
  std::unordered_map<std::string, std::vector<pybind11::object>> callbacks_;
  uint32_t last_tick_count_;
  ui::WindowManager window_mgr_;
  std::vector<std::string> log_lines_;
  RagnarokClient client_;
  std::vector<std::string> loaded_plugins_;
};
