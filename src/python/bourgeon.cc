#include "bourgeon.h"

#include <iostream>

#include <pybind11/embed.h>

namespace python {

PYBIND11_EMBEDDED_MODULE(bourgeon, m) {
  // Log function
  m.def("log", [](std::string message) { std::cout << message << std::endl; });
  // Best function
  m.def("register_callback", [](std::string cb_name, pybind11::object func) {
    Bourgeon::Instance().RegisterCallback(cb_name, func);
  });
  m.def("unregister_callback", [](std::string cb_name, pybind11::object func) {
    Bourgeon::Instance().UnregisterCallback(cb_name, func);
  });
}

}  // namespace python
