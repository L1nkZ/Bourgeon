#pragma once

#include <string>
#include <vector>

#include "ui/widget.h"

namespace ui {

class Text final : public Widget {
 public:
  Text(std::string text) : Widget(), text_(std::move(text)) {}
  ~Text() override = default;

  const std::string& text() const;
  void set_text(std::string value);
  void Show() override;

 private:
  std::string text_;
};

}  // namespace ui