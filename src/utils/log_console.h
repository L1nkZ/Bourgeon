#pragma once

#include <memory>
#include <string>

#include "spdlog/spdlog.h"

class LogConsole {
 public:
  LogConsole(LogConsole const&) = delete;
  void operator=(LogConsole const&) = delete;
  static LogConsole& instance() {
    // Guaranteed to be destroyed.
    // Instantiated on first use.
    static LogConsole instance;

    return instance;
  }

  void Info(const std::string& info);
  void Error(const std::string& error);
  void Debug(const std::string& log);

 private:
  LogConsole();
  ~LogConsole();

 private:
  bool should_free_console_;
  std::shared_ptr<spdlog::logger> p_logger_;
};

#define LogInfo(fmt, ...) LogConsole::instance().Info(fmt, ##__VA_ARGS__)

#define LogError(fmt, ...) LogConsole::instance().Error(fmt, ##__VA_ARGS__)

#define LogDebug(fmt, ...) LogConsole::instance().Debug(fmt, ##__VA_ARGS__)
