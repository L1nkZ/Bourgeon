#include "bourgeon.h"

#include <pybind11/embed.h>

#include <iostream>

namespace python {

PYBIND11_EMBEDDED_MODULE(bourgeon, m) {
  // Log function
  m.def("log", [](std::string message) {
    Bourgeon::Instance().AddLogLine(std::move(message));
  });
  // Best function
  m.def("register_callback", [](std::string cb_name, pybind11::object func) {
    Bourgeon::Instance().RegisterCallback(cb_name, func);
  });
  m.def("unregister_callback", [](std::string cb_name, pybind11::object func) {
    Bourgeon::Instance().UnregisterCallback(cb_name, func);
  });
}

}  // namespace python
