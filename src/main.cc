#include <Windows.h>

BOOL bourgeon_main() {

  return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      return bourgeon_main();
  };

  return TRUE;
}