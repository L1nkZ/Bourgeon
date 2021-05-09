#include <Windows.h>

#include "bourgeon.h"
#include "ddraw/ddraw.h"

BOOL WINAPI DllMain(HINSTANCE hinst_dll, DWORD fdw_reason,
                    LPVOID lpv_reserved) {
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

      break;
    case DLL_PROCESS_DETACH:
      FreeDDraw();

      break;
    default:
      break;
  };

  return TRUE;
}
