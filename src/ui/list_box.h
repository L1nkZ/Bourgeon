#pragma once

#include <string>
#include <vector>

#include "ui/widget.h"
#include "ui/window.h"

namespace ui {

class ListBox final : public Widget {
 public:
  ListBox(std::string label, std::vector<std::string> values,
          MessageId on_select)
      : Widget(),
        label_(std::move(label)),
        values_(std::move(values)),
        on_select_(on_select),
        selected_item_(),
        size_(),
        disabled_() {}
  ~ListBox() override = default;

  void Show() override;

  inline const std::string& label() const { return label_; }
  inline void set_label(std::string value) { label_ = std::move(value); }

  inline std::pair<float, float> size() const { return {size_.x, size_.y}; }
  inline void set_size(std::pair<float, float> value) {
    size_ = ImVec2(value.first, value.second);
  }

  inline bool disabled() const { return disabled_; }
  inline void set_disabled(bool value) { disabled_ = value; }

 private:
  void UpdateSelectedItem(int32_t selected_item);

  std::string label_;
  std::vector<std::string> values_;
  MessageId on_select_;
  size_t selected_item_;
  ImVec2 size_;
  bool disabled_;
};

}  // namespace ui