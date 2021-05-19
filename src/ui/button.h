#pragma once

#include <string>
#include <vector>

#include "ui/widget.h"
#include "ui/window.h"

namespace ui {

class Button final : public Widget {
 public:
  Button(std::string label, MessageId on_click)
      : Widget(), label_(std::move(label)), on_click_(on_click) {}
  ~Button() override = default;

  const std::string& label() const;
  void set_label(std::string value);
  void Show() override;

 private:
  std::string label_;
  MessageId on_click_;
};

}  // namespace ui