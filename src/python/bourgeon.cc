#include "bourgeon.h"

#include <pybind11/embed.h>
#include <pybind11/stl.h>

#include "ui/button.h"
#include "ui/check_box.h"
#include "ui/combo.h"
#include "ui/list_box.h"
#include "ui/separator.h"
#include "ui/text.h"
#include "ui/text_input.h"
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

  pybind11::class_<ui::Separator, ui::Widget, std::shared_ptr<ui::Separator>>(
      m_ui, "Separator")
      .def(pybind11::init<>());

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

  pybind11::class_<ui::TextInput, ui::Widget, std::shared_ptr<ui::TextInput>>(
      m_ui, "TextInput")
      .def(pybind11::init<std::string, std::string, ui::TextInput::AllowedChars,
                          size_t, ui::MessageId>())
      .def("label", &ui::TextInput::label)
      .def("set_label", &ui::TextInput::set_label)
      .def("width", &ui::TextInput::width)
      .def("set_width", &ui::TextInput::set_width)
      .def("read_only", &ui::TextInput::read_only)
      .def("set_read_only", &ui::TextInput::set_read_only);

  pybind11::enum_<ui::TextInput::AllowedChars>(m_ui, "AllowedChars")
      .value("All", ui::TextInput::AllowedChars::kAll)
      .value("Decimal", ui::TextInput::AllowedChars::kDecimal)
      .value("Hexadecimal", ui::TextInput::AllowedChars::kHexadecimal)
      .value("NoBlanks", ui::TextInput::AllowedChars::kNoBlanks)
      .value("Scientific", ui::TextInput::AllowedChars::kScientific);

  pybind11::class_<ui::Combo, ui::Widget, std::shared_ptr<ui::Combo>>(m_ui,
                                                                      "Combo")
      .def(pybind11::init<std::string, std::vector<std::string>,
                          ui::MessageId>())
      .def("label", &ui::Combo::label)
      .def("set_label", &ui::Combo::set_label)
      .def("disabled", &ui::Combo::disabled)
      .def("set_disabled", &ui::Combo::set_disabled);

  pybind11::class_<ui::ListBox, ui::Widget, std::shared_ptr<ui::ListBox>>(
      m_ui, "ListBox")
      .def(pybind11::init<std::string, std::vector<std::string>,
                          ui::MessageId>())
      .def("label", &ui::ListBox::label)
      .def("set_label", &ui::ListBox::set_label)
      .def("size", &ui::ListBox::size)
      .def("set_size", &ui::ListBox::set_size)
      .def("disabled", &ui::ListBox::disabled)
      .def("set_disabled", &ui::ListBox::set_disabled);
}

}  // namespace python
