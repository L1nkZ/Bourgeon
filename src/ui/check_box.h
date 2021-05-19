#pragma once

#include <string>
#include <vector>

#include "ui/widget.h"
#include "ui/window.h"

namespace ui {

class CheckBox final : public Widget {
 public:
  CheckBox(std::string label, bool checked, MessageId on_check)
      : Widget(),
        label_(std::move(label)),
        checked_(checked),
        on_check_(on_check) {}
  ~CheckBox() override = default;

  const std::string& label() const;
  void set_label(std::string value);
  void Show() override;

 private:
  std::string label_;
  bool checked_;
  MessageId on_check_;
};

}  // namespace ui