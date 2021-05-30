#pragma once

#include <string>
#include <vector>

#include "ui/widget.h"
#include "ui/window.h"

namespace ui {

class Combo final : public Widget {
 public:
  Combo(std::string label, std::vector<std::string> values, MessageId on_select)
      : Widget(),
        label_(std::move(label)),
        values_(std::move(values)),
        on_select_(on_select),
        selected_item_(),
        disabled_() {}
  ~Combo() override = default;

  const std::string& label() const;
  void set_label(std::string value);
  bool disabled() const;
  void set_disabled(bool value);
  void Show() override;

 private:
  void UpdateSelectedItem(int32_t selected_item);

  std::string label_;
  std::vector<std::string> values_;
  MessageId on_select_;
  size_t selected_item_;
  bool disabled_;
};

}  // namespace ui