#pragma once

#include <cstdint>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "imgui.h"
#include "pybind11/pytypes.h"
#include "ui/widget.h"

namespace ui {

using WindowLayout = std::vector<std::vector<std::shared_ptr<Widget>>>;
using MessageId = int32_t;
using Message = std::pair<MessageId, pybind11::tuple>;
using MessageQueue = std::list<Message>;

class Window final : public Widget,
                     public std::enable_shared_from_this<Window> {
 public:
  Window(std::string name, WindowLayout layout, MessageId on_close);
  ~Window() override = default;

  void Show() override;

  void Initialize();
  void PostMessage(Message message);
  std::optional<Message> GetMessage();

  inline std::pair<float, float> size() const { return {size_.x, size_.y}; }
  inline void set_size(std::pair<float, float> value) {
    size_ = ImVec2(value.first, value.second);
  }
  inline bool movable() const { return movable_; }
  inline void set_movable(bool value) { movable_ = value; }
  inline bool resizable() const { return resizable_; }
  inline void set_resizable(bool value) { resizable_ = value; }

 private:
  std::string name_;
  WindowLayout layout_;
  MessageId on_close_;
  MessageQueue message_queue_;
  ImVec2 size_;
  bool movable_;
  bool resizable_;
};

}  // namespace ui