#include <Windows.h>

#include "bourgeon.h"
#include "ddraw/ddraw.h"

DWORD WINAPI bourgeon_tick(LPVOID) {
  Bourgeon::Instance().RunTick();
  return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst_dll, DWORD fdw_reason,
                    LPVOID lpv_reserved) {
  static HANDLE bourgeon_thread = nullptr;

  switch (fdw_reason) {
    case DLL_PROCESS_ATTACH:
      DisableThreadLibraryCalls(hinst_dll);

      // Load the original ddraw DLL
      if (!LoadDDraw()) {
        return FALSE;
      }

      // Initialize Bourgeon (Create hooks and load plugins)
      if (!Bourgeon::Instance().Initialize()) {
        // Note: Return TRUE to stay in memory and let the client start properly
        return TRUE;
      }

      bourgeon_thread =
          CreateThread(nullptr, 0, &bourgeon_tick, nullptr, 0, nullptr);
      break;
    case DLL_PROCESS_DETACH:
      FreeDDraw();
      if (bourgeon_thread != nullptr) {
        TerminateThread(bourgeon_thread, 0);
      }
      break;
    default:
      break;
  };

  return TRUE;
}
