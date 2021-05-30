#pragma once

#include "ui/widget.h"

namespace ui {

class Separator final : public Widget {
 public:
  Separator() : Widget() {}
  ~Separator() override = default;

  void Show() override;
};

}  // namespace ui