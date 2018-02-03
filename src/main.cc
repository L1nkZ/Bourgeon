#include "bourgeon.h"

#include <Windows.h>

static HANDLE bourgeon_thread;

void bourgeon_tick() { Bourgeon::Instance().RunTick(); }

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hinstDLL);

      // Initialize Bourgeon (Create hooks and load plugins)
      if (!Bourgeon::Instance().Initialize()) {
        return FALSE;
      }
      bourgeon_thread =
          CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&bourgeon_tick,
                       nullptr, 0, nullptr);
      break;
    case DLL_PROCESS_DETACH:
      TerminateThread(bourgeon_thread, 0);
  };

  return TRUE;
}