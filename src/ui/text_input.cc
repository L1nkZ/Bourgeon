#include "ui/text_input.h"

#include "imgui.h"
#include "pybind11/pybind11.h"

namespace ui {

const std::string &TextInput::label() const { return label_; }

void TextInput::set_label(std::string value) { label_ = std::move(value); }

float TextInput::width() const { return width_; }

void TextInput::set_width(float value) { width_ = value; }

bool TextInput::read_only() const { return read_only_; }

void TextInput::set_read_only(bool value) { read_only_ = value; }

void TextInput::Show() {
  if (width_ > 0.0F) {
    // Custom width
    ImGui::SetNextItemWidth(width_);
  }

  // Setup flags
  ImGuiInputTextFlags flags{};
  if (read_only_) {
    flags |= ImGuiInputTextFlags_ReadOnly;
  }
  switch (allowed_chars_) {
    case AllowedChars::kDecimal:
      flags |= ImGuiInputTextFlags_CharsDecimal;
      break;
    case AllowedChars::kHexadecimal:
      flags |= ImGuiInputTextFlags_CharsHexadecimal;
      break;
    case AllowedChars::kUppercase:
      flags |= ImGuiInputTextFlags_CharsUppercase;
      break;
    case AllowedChars::kNoBlanks:
      flags |= ImGuiInputTextFlags_CharsNoBlank;
      break;
    case AllowedChars::kScientific:
      flags |= ImGuiInputTextFlags_CharsScientific;
      break;
    default:
      break;
  }

  if (ImGui::InputText(label_.c_str(), buffer_.data(), buffer_.size(), flags)) {
    if (auto p_parent = p_parent_.lock()) {
      p_parent->PostMessage({on_input_, pybind11::make_tuple(buffer_.data())});
    }
  }
}

}  // namespace ui