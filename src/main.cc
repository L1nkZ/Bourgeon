#include "core/bourgeon.h"

#include <Windows.h>

static HANDLE bourgeon_thread;

void bourgeon_main() { Bourgeon::Instance().Initialize(); }

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hinstDLL);
      bourgeon_thread =
          CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&bourgeon_main,
                       nullptr, 0, nullptr);
      break;
    case DLL_PROCESS_DETACH:
      TerminateThread(bourgeon_thread, 0);
  };

  return TRUE;
}