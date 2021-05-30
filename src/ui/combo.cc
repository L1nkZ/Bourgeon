#include "ui/combo.h"

#include "imgui.h"
#include "pybind11/cast.h"

namespace ui {

const std::string& Combo::label() const { return label_; }

void Combo::set_label(std::string value) { label_ = std::move(value); }

bool Combo::disabled() const { return disabled_; }

void Combo::set_disabled(bool value) { disabled_ = value; }

void Combo::Show() {
  ImGuiSelectableFlags flags{};
  if (disabled_) {
    flags |= ImGuiSelectableFlags_Disabled;
  }

  if (ImGui::BeginCombo(label_.c_str(), values_[selected_item_].c_str())) {
    for (size_t n = 0; n < values_.size(); n++) {
      if (ImGui::Selectable(values_[n].c_str(), selected_item_ == n, flags)) {
        UpdateSelectedItem(n);
      }

      // Set the initial focus when opening the combo (scrolling + keyboard
      // navigation focus)
      if (selected_item_ == n) {
        ImGui::SetItemDefaultFocus();
      }
    }

    ImGui::EndCombo();
  }
}

void Combo::UpdateSelectedItem(int32_t selected_item) {
  selected_item_ = selected_item;
  if (auto p_parent = p_parent_.lock()) {
    p_parent->PostMessage({on_select_, pybind11::make_tuple(selected_item_)});
  }
}

}  // namespace ui