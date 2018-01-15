#ifndef BOURGEON_UTILS_LOG_CONSOLE_H_
#define BOURGEON_UTILS_LOG_CONSOLE_H_

#include <string>

class LogConsole {
 public:
  LogConsole(bool alloc_console = true);
  ~LogConsole();

  void LogInfo(const std::string& info);
  void LogError(const std::string& error);
  void LogDebug(const std::string& log);

 private:
  FILE* console_fp_;
};

#endif /* BOURGEON_UTILS_LOG_CONSOLE_H_ */