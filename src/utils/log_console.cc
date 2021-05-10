#include "utils/log_console.h"

#include <Windows.h>

#include <iostream>
#include <vector>

#include "spdlog/sinks/msvc_sink.h"

LogConsole::LogConsole() {
  if (AllocConsole() == TRUE) {
    FILE *out;
    freopen_s(&out, "CONOUT$", "w", stdout);
  }

  auto stdout_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
  p_logger_ =
      std::make_unique<spdlog::logger>("Bourgeon", std::move(stdout_sink));

#ifdef BOURGEON_DEBUG
  p_logger_->set_level(spdlog::level::debug);
#else
  p_logger_->set_level(spdlog::level::info);
#endif
}

LogConsole::~LogConsole() {}

spdlog::logger *LogConsole::logger() const { return p_logger_.get(); }
