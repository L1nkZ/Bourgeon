#include "native_hooks.h"
#include <pybind11/embed.h>
#include <pybind11/pytypes.h>
#include <iostream>
#include "core/bourgeon.h"
#include "ragnarok/talktype.h"
#include "utils/hooking/hook_manager.h"
#include "utils/log_console.h"

#include <Windows.h>

namespace native_hooks {
// Prototypes
int WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  return 0;
};

bool _fastcall ProcessPushButton(int, int, unsigned long, int, int) {
  return false;
};

void _fastcall Zc_Notify_Playerchat(int, int, const char *){};

int _fastcall GetTalkType(int, int, char const *chat_buffer,
                          TalkType *talk_type, void *) {
  return 0;
}

using namespace hooking;

// Nasty stuff
#define CallRegistrees(CB_NAME, ...)                                       \
  {                                                                        \
    auto registrees = Bourgeon::Instance().GetCallbackRegistrees(CB_NAME); \
    for (auto registree : registrees) {                                    \
      try {                                                                \
        registree(__VA_ARGS__);                                            \
      } catch (pybind11::error_already_set & error) {                      \
        std::cerr << error.what() << std::endl;                            \
        Bourgeon::Instance().UnregisterCallback(CB_NAME, registree);       \
      }                                                                    \
    }                                                                      \
  }

static std::unordered_map<void *, void *> destinations;
static std::unordered_map<std::string, uint8_t *> hook_addresses = {
    {"WindowProc", (uint8_t *)0x00AA7370},     // WindowProc
    {"OnKeyDown", (uint8_t *)0x006BA3F0},      // ProcessPushButton
    {"OnChatMessage", (uint8_t *)0x009E62B0},  // Zc_Notify_Playerchat
    {"OnTalkType", (uint8_t *)0x00A5E960}      // GetTalkType
};

// int HWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
//  switch (message) {
//    case 0x104:
//      CallRegistrees("OnKeyDown", wParam, ~(lParam >> 30) & 1, 0);
//      break;
//    case 0x100:
//      CallRegistrees("OnKeyDown", wParam, ~(lParam >> 30) & 1, lParam);
//      break;
//  }
//
//  decltype(&WindowProc) OWindowProc =
//      reinterpret_cast<decltype(&WindowProc)>(destinations[HWindowProc]);
//  return OWindowProc(hWnd, message, wParam, lParam);
//}
//
// void HookWindowProc() {
//  destinations[HWindowProc] = HookManager::Instance().SetHook(
//      HookType::kJmpHook, hook_addresses["WindowProc"],
//      reinterpret_cast<uint8_t *>(HWindowProc));
//}

// OnKeyDown
bool _fastcall OnKeyDown(int ecx, int edx, unsigned long vkey, int new_key_down,
                         int accurate_key) {
  CallRegistrees(__func__, vkey, new_key_down, accurate_key);
  decltype(&ProcessPushButton) OProcessPushButton =
      reinterpret_cast<decltype(&ProcessPushButton)>(destinations[OnKeyDown]);
  return OProcessPushButton(ecx, edx, vkey, new_key_down, accurate_key);
}

void HookOnKeyDown() {
  destinations[OnKeyDown] = HookManager::Instance().SetHook(
      HookType::kJmpHook, hook_addresses["OnKeyDown"],
      reinterpret_cast<uint8_t *>(OnKeyDown));
}

// OnChatMessage
void _fastcall OnChatMessage(int ecx, int edx, const char *packet) {
  std::string message(packet + 4);
  try {
    pybind11::str py_s = pybind11::reinterpret_steal<pybind11::str>(
        PyUnicode_DecodeLatin1(message.data(), message.length(), nullptr));
    CallRegistrees(__func__, py_s);
  } catch (pybind11::error_already_set &error) {
    std::cerr << error.what() << std::endl;
  }

  decltype(&Zc_Notify_Playerchat) OProcessPushButton =
      reinterpret_cast<decltype(&Zc_Notify_Playerchat)>(
          destinations[OnChatMessage]);
  return OProcessPushButton(ecx, edx, packet);
}

void HookOnChatMessage() {
  destinations[OnChatMessage] = HookManager::Instance().SetHook(
      HookType::kJmpHook, hook_addresses["OnChatMessage"],
      reinterpret_cast<uint8_t *>(OnChatMessage));
}

// OnTalkType
int _fastcall OnTalkType(int ecx, int edx, char const *chat_buffer,
                         TalkType *talk_type, void *param) {
  try {
    pybind11::str py_chat = pybind11::reinterpret_steal<pybind11::str>(
        PyUnicode_DecodeLatin1(chat_buffer, strlen(chat_buffer), nullptr));
    CallRegistrees(__func__, py_chat);
  } catch (pybind11::error_already_set &error) {
    std::cerr << error.what() << std::endl;
  }

  decltype(&GetTalkType) OProcessPushButton =
      reinterpret_cast<decltype(&GetTalkType)>(destinations[OnTalkType]);
  return OProcessPushButton(ecx, edx, chat_buffer, talk_type, param);
}

void HookOnTalkType() {
  destinations[OnTalkType] = HookManager::Instance().SetHook(
      HookType::kJmpHook, hook_addresses["OnTalkType"],
      reinterpret_cast<uint8_t *>(OnTalkType));
}

}  // namespace native_hooks