#include "ui/window.h"

#include "pybind11/cast.h"

namespace ui {

Window::Window(std::string name, WindowLayout layout, MessageId on_close)
    : Widget(),
      name_(std::move(name)),
      layout_(std::move(layout)),
      on_close_(on_close),
      message_queue_(),
      size_(),
      movable_(true),
      resizable_(true) {}

void Window::Initialize() {
  for (auto& row : layout_) {
    for (auto& widget : row) {
      widget->set_parent(shared_from_this());
    }
  }
}

void Window::Show() {
  ImGui::SetNextWindowSize(size_);

  ImGuiWindowFlags flags{};
  if (!movable_) {
    flags |= ImGuiWindowFlags_NoMove;
  }
  if (!resizable_) {
    flags |= ImGuiWindowFlags_NoResize;
  }

  bool window_open = true;
  if (ImGui::Begin(name_.c_str(), &window_open, flags)) {
    for (auto& row : layout_) {
      const auto row_size = row.size();
      for (size_t i = 0; i < row_size; i++) {
        const auto& widget = row[i];
        widget->Show();
        if (i < row_size - 1) {
          ImGui::SameLine();
        }
      }
    }
  }
  ImGui::End();

  if (!window_open) {
    PostMessage({on_close_, pybind11::make_tuple()});
  }
}

void Window::PostMessage(Message message) {
  message_queue_.emplace_back(std::move(message));
}

std::optional<Message> Window::GetMessage() {
  if (message_queue_.empty()) {
    return std::nullopt;
  }

  const auto msg = std::move(message_queue_.front());
  message_queue_.pop_front();
  return msg;
}

}  // namespace ui