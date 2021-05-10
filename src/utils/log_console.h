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

  spdlog::logger* logger() const;

 private:
  LogConsole();
  ~LogConsole();

 private:
  std::unique_ptr<spdlog::logger> p_logger_;
};

#define LogInfo(fmt, ...) \
  LogConsole::instance().logger()->info(fmt, ##__VA_ARGS__)

#define LogError(fmt, ...) \
  LogConsole::instance().logger()->error(fmt, ##__VA_ARGS__)

#define LogDebug(fmt, ...) \
  LogConsole::instance().logger()->debug(fmt, ##__VA_ARGS__)
