#include "bourgeon.h"

#include <Windows.h>

static HANDLE bourgeon_thread;

void bourgeon_tick() { Bourgeon::Instance().RunTick(); }

BOOL WINAPI DllMain(HINSTANCE hinst_dll, DWORD fdw_reason,
                    LPVOID lpv_reserved) {
  switch (fdw_reason) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hinst_dll);

      // Initialize Bourgeon (Create hooks and load plugins)
      if (!Bourgeon::Instance().Initialize()) {
        return FALSE;
      }

      bourgeon_thread = CreateThread(
          NULL, 0, (LPTHREAD_START_ROUTINE)&bourgeon_tick, NULL, 0, NULL);
      break;
    case DLL_PROCESS_DETACH:
      TerminateThread(bourgeon_thread, 0);
      break;
  };

  return TRUE;
}
