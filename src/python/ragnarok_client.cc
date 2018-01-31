#include <pybind11/embed.h>
#include "core/bourgeon.h"

namespace python {

PYBIND11_EMBEDDED_MODULE(ragnarok_client, m) {
  // Print a message into the game chat
  m.def("print_in_chat",
        [](std::string message, unsigned int color, unsigned int filter) {
          Bourgeon::Instance().client().window_mgr().SendMsg(
              UIMessage::UIM_PUSHINTOCHATHISTORY,
              reinterpret_cast<int>(message.c_str()), color, filter, 0);
        });

  // Use an item given its id. Returns true if the item was used successfully,
  // and false otherwise.
  m.def("use_item", [](int item_id) -> bool {
    return Bourgeon::Instance().client().UseItemById(item_id);
  });

  m.def("get_hp", []() -> int {
    return Bourgeon::Instance().client().session().GetHp();
  });
  m.def("get_max_hp", []() -> int {
    return Bourgeon::Instance().client().session().GetMaxHp();
  });
  m.def("get_sp", []() -> int {
    return Bourgeon::Instance().client().session().GetSp();
  });
  m.def("get_max_sp", []() -> int {
    return Bourgeon::Instance().client().session().GetMaxSp();
  });
}

}  // namespace python