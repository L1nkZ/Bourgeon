#include "log_console.h"
#include <Windows.h>
#include <iostream>

LogConsole::LogConsole(bool alloc_console) : cout_fp_(), cerr_fp_() {
  if (alloc_console && AllocConsole()) {
    freopen_s(&cout_fp_, "CONOUT$", "w", stdout);
    freopen_s(&cerr_fp_, "CONERR$", "w", stderr);
  }
}

LogConsole::~LogConsole() {
  if (cout_fp_ || cerr_fp_) FreeConsole();

  if (cout_fp_) fclose(cout_fp_);
  if (cerr_fp_) fclose(cerr_fp_);
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