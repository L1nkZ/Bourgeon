#define NOMINMAX

#include <Windows.h>
#include <pybind11/embed.h>
#include "core/ragnarok_client.h"
#include "utils/log_console.h"

namespace py = pybind11;

BOOL bourgeon_main() {
  LogConsole console;
  RagnarokClient client;

  if (!client.Initialize()) {
    console.LogError("Bourgeon failed to initialize");
    return FALSE;
  }

  console.LogInfo("Bourgeon initialized successfully !");
  console.LogInfo("Detected client: " + std::to_string(client.timestamp()));

  return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      return bourgeon_main();
  };

  return TRUE;
}