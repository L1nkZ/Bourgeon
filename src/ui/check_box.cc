#include "ui/check_box.h"

#include "imgui.h"
#include "pybind11/cast.h"

namespace ui {

const std::string& CheckBox::label() const { return label_; }

void CheckBox::set_label(std::string value) { label_ = std::move(value); }

void CheckBox::Show() {
  if (ImGui::Checkbox(label_.c_str(), &checked_)) {
    if (auto p_parent = p_parent_.lock()) {
      p_parent->PostMessage({on_check_, pybind11::make_tuple(checked_)});
    }
  }
}

}  // namespace ui