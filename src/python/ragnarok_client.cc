#include <pybind11/embed.h>
#include "core/bourgeon.h"

namespace python {

void print_message(const char *message, unsigned int color,
                   unsigned int filter) {
  uint32_t UIChatWnd_SendMsg = 0x006BCFC0;

  _asm
  {
      push 00h
      push filter /* Filter */
      push color /* Color */
      push message /* Message */
      push 01h
      mov ECX, 0x00E53F00
      call UIChatWnd_SendMsg
  }
}

PYBIND11_EMBEDDED_MODULE(ragnarok_client, m) {
  // Print a message into the game chat
  m.def("print_in_chat", [](std::string message, unsigned int color = 0xFFFFFF,
                            unsigned int filter = 0) {
    print_message(message.c_str(), color, filter);
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