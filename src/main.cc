#include "core/bourgeon.h"

#include <Windows.h>

void bourgeon_main() { Bourgeon::Instance().Initialize(); }

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hinstDLL);
      CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&bourgeon_main, nullptr,
                   0, nullptr);
  };

  return TRUE;
}