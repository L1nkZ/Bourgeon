#include "py_callbacks.h"
#include <pybind11/embed.h>
#include <iostream>
#include "core/bourgeon.h"
#include "utils/log_console.h"

namespace pycallbacks {

PYBIND11_EMBEDDED_MODULE(bourgeon, m) {
  m.def("log", [](std::string message) { std::cout << message << std::endl; });
}

PYBIND11_EMBEDDED_MODULE(rocallbacks, m) {
  m.def("RegisterCallback", [](std::string cb_name, pybind11::object func) {
    Bourgeon::Instance().RegisterCallback(cb_name, func);
  });
}

bool _fastcall OnKeyDown(size_t ecx, size_t edx, unsigned long vkey,
                         int new_key_down, int accurate_key) {
  auto registrees = Bourgeon::Instance().GetCallbackRegistrees("OnKeyDown");
  for (auto registree : registrees) {
    registree(vkey, new_key_down, accurate_key);
  }

  return true;
}

}  // namespace pycallbacks