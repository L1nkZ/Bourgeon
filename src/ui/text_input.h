#pragma once

#include <string>
#include <vector>

#include "ui/widget.h"
#include "ui/window.h"

namespace ui {

class TextInput final : public Widget {
 public:
  enum class AllowedChars : int {
    kAll = 0,
    kDecimal,
    kHexadecimal,
    kUppercase,
    kNoBlanks,
    kScientific
  };

  TextInput(std::string label, std::string default_text,
            AllowedChars allowed_chars, size_t max_chars, MessageId on_input)
      : Widget(),
        label_(std::move(label)),
        allowed_chars_(allowed_chars),
        on_input_(on_input),
        width_(-1.0F),
        read_only_() {
    buffer_.resize(max_chars + 1);
    strncpy_s(buffer_.data(), buffer_.size(), default_text.c_str(), _TRUNCATE);
  }
  ~TextInput() override = default;

  const std::string& label() const;
  void set_label(std::string value);
  float width() const;
  void set_width(float value);
  bool read_only() const;
  void set_read_only(bool value);
  void Show() override;

 private:
  std::vector<char> buffer_;
  std::string label_;
  AllowedChars allowed_chars_;
  MessageId on_input_;
  float width_;
  bool read_only_;
};

}  // namespace ui