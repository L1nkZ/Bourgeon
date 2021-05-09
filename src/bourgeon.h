#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "pybind11/embed.h"
#include "ragnarok/ragnarok_client.h"

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

  bool Initialize();
  void OnTick();

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

  pybind11::scoped_interpreter interpreter_;
  std::unordered_map<std::string, std::vector<pybind11::object>> callbacks_;
  RagnarokClient client_;
  uint32_t last_tick_count_;
};
