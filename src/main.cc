#include <Windows.h>

#include "bourgeon.h"

DWORD WINAPI bourgeon_tick(LPVOID) {
  Bourgeon::Instance().RunTick();
  return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst_dll, DWORD fdw_reason,
                    LPVOID lpv_reserved) {
  static HANDLE bourgeon_thread;

  switch (fdw_reason) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hinst_dll);

      // Initialize Bourgeon (Create hooks and load plugins)
      if (!Bourgeon::Instance().Initialize()) {
        return FALSE;
      }

      bourgeon_thread =
          CreateThread(nullptr, 0, &bourgeon_tick, nullptr, 0, nullptr);
      break;
    case DLL_PROCESS_DETACH:
      TerminateThread(bourgeon_thread, 0);
      break;
    default:
      break;
  };

  return TRUE;
}
