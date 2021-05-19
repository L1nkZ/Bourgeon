#include "bourgeon.h"

#include <pybind11/embed.h>
#include <pybind11/stl.h>

#include <iostream>

#include "ui/button.h"
#include "ui/check_box.h"
#include "ui/text.h"
#include "ui/window.h"

namespace python {

PYBIND11_EMBEDDED_MODULE(bourgeon, m) {
  // Log function
  m.def("log", [](std::string message) {
    Bourgeon::Instance().AddLogLine(std::move(message));
  });

  // Callback registration
  m.def("register_callback", [](std::string cb_name, pybind11::object func) {
    Bourgeon::Instance().RegisterCallback(cb_name, func);
  });
  m.def("unregister_callback", [](std::string cb_name, pybind11::object func) {
    Bourgeon::Instance().UnregisterCallback(cb_name, func);
  });

  // UI
  pybind11::module_ m_ui = m.def_submodule("ui", "Bourgeon's UI submodule");
  m_ui.def("register_window", [](std::shared_ptr<ui::Window> p_window) {
    Bourgeon::Instance().window_manager().RegisterWindow(std::move(p_window));
  });
  m_ui.def("unregister_window", [](std::shared_ptr<ui::Window> p_window) {
    Bourgeon::Instance().window_manager().UnregisterWindow(std::move(p_window));
  });
  pybind11::class_<ui::Widget, std::shared_ptr<ui::Widget>>(m_ui, "Widget")
      .def(pybind11::init<>());
  pybind11::class_<ui::Window, std::shared_ptr<ui::Window>>(m_ui, "Window")
      .def(pybind11::init<std::string, ui::WindowLayout, ui::MessageId>())
      .def("read", &ui::Window::GetMessage)
      .def("movable", &ui::Window::movable)
      .def("set_movable", &ui::Window::set_movable)
      .def("resizable", &ui::Window::resizable)
      .def("set_resizable", &ui::Window::set_resizable)
      .def("size", &ui::Window::size)
      .def("set_size", &ui::Window::set_size);
  pybind11::class_<ui::Text, ui::Widget, std::shared_ptr<ui::Text>>(m_ui,
                                                                    "Text")
      .def(pybind11::init<std::string>())
      .def("text", &ui::Text::text)
      .def("set_text", &ui::Text::set_text);
  pybind11::class_<ui::Button, ui::Widget, std::shared_ptr<ui::Button>>(
      m_ui, "Button")
      .def(pybind11::init<std::string, ui::MessageId>())
      .def("label", &ui::Button::label)
      .def("set_label", &ui::Button::set_label);
  pybind11::class_<ui::CheckBox, ui::Widget, std::shared_ptr<ui::CheckBox>>(
      m_ui, "CheckBox")
      .def(pybind11::init<std::string, bool, ui::MessageId>())
      .def("label", &ui::CheckBox::label)
      .def("set_label", &ui::CheckBox::set_label);
}

}  // namespace python
