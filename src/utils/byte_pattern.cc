#include "byte_pattern.h"

BytePattern::BytePattern(const std::vector<uint8_t>& pattern,
                         const std::string& mask) {
  pattern_ = pattern;
  mask_ = mask;
}

void* BytePattern::Search(void* address, size_t len) {
  uint8_t* haystack = reinterpret_cast<uint8_t*>(address);
  size_t scan, nlen = mask_.length();
  size_t bad_char_skip[256];

  for (scan = 0; scan < 256; scan++) bad_char_skip[scan] = nlen;

  size_t last = nlen - 1;

  for (scan = 0; scan < last; scan++)
    if (pattern_[scan] != '?') bad_char_skip[pattern_[scan]] = last - scan;

  while (len >= nlen) {
    for (scan = last; mask_[scan] == '?' || haystack[scan] == pattern_[scan];
         scan--)
      if (scan == 0) return haystack;

    len -= bad_char_skip[haystack[last]];
    haystack += bad_char_skip[haystack[last]];
  }

  return nullptr;
}

bool BytePattern::Match(void* address, size_t offset) { return false; }

size_t BytePattern::GetSize() { return pattern_.size(); }
