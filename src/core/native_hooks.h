#ifndef BOURGEON_CORE_PY_CALLBACKS_H_
#define BOURGEON_CORE_PY_CALLBACKS_H_

#include <functional>
#include <unordered_map>

namespace native_hooks {

// void HookWindowProc();
void HookOnKeyDown();
void HookOnChatMessage();

}  // namespace native_hooks

#endif  // BOURGEON_CORE_PY_CALLBACKS_H_