#ifndef BOURGEON_CORE_PY_CALLBACKS_H_
#define BOURGEON_CORE_PY_CALLBACKS_H_

namespace pycallbacks {
bool _fastcall OnKeyDown(size_t ecx, size_t edx, unsigned long virtualKey,
                         int newKeydown, int accurateKey);
}

#endif /* BOURGEON_CORE_PY_CALLBACKS_H_ */