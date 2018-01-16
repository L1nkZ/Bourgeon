#ifndef BOURGEON_CORE_RAGNAROK_CLIENT_H_
#define BOURGEON_CORE_RAGNAROK_CLIENT_H_

#include <string>

class RagnarokClient {
 public:
  RagnarokClient();

  bool Initialize();

  unsigned long timestamp() const;

 private:
  unsigned long GetClientTimeStamp() const;
  void* GetClientBase() const;
  std::string GetClientFilename() const;
  unsigned long TranslateTimeStamp(const std::string& str) const;
  bool RegisterHooks();

 public:
  const unsigned long kUnknownTimeStamp = 0;

 private:
  unsigned long timestamp_;
};

#endif /* BOURGEON_CORE_RAGNAROK_CLIENT_H_ */