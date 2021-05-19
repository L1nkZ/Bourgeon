#pragma once

#include <memory>

namespace ui {

class Window;

class Widget {
 public:
  virtual ~Widget() {}
  virtual void Show(){};

  void set_parent(std::weak_ptr<Window> p_parent) {
    p_parent_ = std::move(p_parent);
  }

 protected:
  std::weak_ptr<Window> p_parent_;
};

}  // namespace ui