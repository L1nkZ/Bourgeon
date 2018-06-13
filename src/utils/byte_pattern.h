#pragma once

#include <string>
#include <vector>

class BytePattern {
 public:
  BytePattern(const std::vector<uint8_t>& pattern, const std::string& mask);

  void* Search(void* address, size_t len);
  bool Match(void* address, size_t offset = 0);
  size_t GetSize();

  std::vector<uint8_t> pattern_;
  std::string mask_;
};
