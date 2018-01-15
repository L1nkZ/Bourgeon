#include "log_console.h"
#include <Windows.h>
#include <iostream>

LogConsole::LogConsole(bool alloc_console) : console_fp_() {
  if (alloc_console && AllocConsole()) {
    freopen_s(&console_fp_, "CONOUT$", "w", stdout);
    freopen_s(&console_fp_, "CONERR$", "w", stderr);
  }
}

LogConsole::~LogConsole() {
  if (console_fp_) {
    fclose(console_fp_);
    FreeConsole();
  }
}

void LogConsole::LogInfo(const std::string& log) {
  std::cout << log << std::endl;
}

void LogConsole::LogError(const std::string& error) {
  std::cerr << error << std::endl;
}

void LogConsole::LogDebug(const std::string& log) {
#ifdef _DEBUG
  std::cout << log << std::endl;
#endif
}