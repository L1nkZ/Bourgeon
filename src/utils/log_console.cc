#include "utils/log_console.h"

#include <Windows.h>

#include <iostream>
#include <vector>

#include "spdlog/sinks/msvc_sink.h"

LogConsole::LogConsole() : should_free_console_() {
  should_free_console_ = AllocConsole() == TRUE;

  auto stdout_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
  p_logger_ = std::make_shared<spdlog::logger>("Bourgeon", stdout_sink);

#ifdef BOURGEON_DEBUG
  p_logger_->set_level(spdlog::level::debug);
#else
  p_logger_->set_level(spdlog::level::info);
#endif
}

LogConsole::~LogConsole() {
  if (should_free_console_) {
    FreeConsole();
  }
}

void LogConsole::Info(const std::string &msg) { p_logger_->info(msg); }

void LogConsole::Error(const std::string &msg) { p_logger_->error(msg); }

void LogConsole::Debug(const std::string &msg) { p_logger_->debug(msg); }
