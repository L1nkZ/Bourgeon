#include "bourgeon.h"

#include <Windows.h>

#define VERSION_MAJOR "0"
#define VERSION_MINOR "1"

Bourgeon::Bourgeon()
    : console_(), interpreter_(), callbacks_(), plugin_threads_(), client_() {}

RagnarokClient& Bourgeon::client() { return client_; }

bool Bourgeon::Initialize() {
  console_.LogInfo("Bourgeon " VERSION_MAJOR "." VERSION_MINOR "\n");

  if (!client_.Initialize()) {
    console_.LogError("Bourgeon failed to initialize");
    return false;
  }
  console_.LogInfo("Bourgeon initialized successfully !");
  console_.LogInfo("Detected client: " + std::to_string(client_.timestamp()));
  console_.LogInfo("Loading plugins ...");
  LoadPlugins("./plugins");

  // Process ticks indefinitely
  auto registrees = callbacks_["Tick"];
  for (;;) {
    for (auto registree : registrees) registree();
    Sleep(1000);
  }

  return true;
}

void Bourgeon::RegisterCallback(const std::string& callback_name,
                                const pybind11::object& function) {
  console_.LogInfo("Registration to " + callback_name + " requested");
  callbacks_[callback_name].push_back(function);
}

const std::vector<pybind11::object>& Bourgeon::GetCallbackRegistrees(
    const std::string& callback_name) {
  return callbacks_[callback_name];
}

void Bourgeon::LoadPlugins(const std::string& folder) {
  using namespace pybind11;

  std::string search_path = folder + "/*.py";
  WIN32_FIND_DATA fd;
  HANDLE h_find = FindFirstFileA(search_path.c_str(), &fd);

  if (h_find == INVALID_HANDLE_VALUE) return;

  do {
    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

    std::string filename(fd.cFileName);
    console_.LogInfo("Found " + filename);
    eval_file(folder + '/' + filename,
              module::import("__main__").attr("__dict__"));
  } while (FindNextFileA(h_find, &fd));

  FindClose(h_find);
}