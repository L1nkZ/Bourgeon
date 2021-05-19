#include "ui/button.h"

#include "imgui.h"
#include "pybind11/cast.h"

namespace ui {

const std::string& Button::label() const { return label_; }

void Button::set_label(std::string value) { label_ = std::move(value); }

void Button::Show() {
  if (ImGui::Button(label_.c_str())) {
    if (auto p_parent = p_parent_.lock()) {
      p_parent->PostMessage({on_click_, pybind11::make_tuple()});
    }
  }
}

}  // namespace ui