#include "bourgeon.h"

#include <Windows.h>

#include "utils/log_console.h"

Bourgeon::Bourgeon()
    : interpreter_(), callbacks_(), client_(), last_tick_count_() {}

RagnarokClient& Bourgeon::client() { return client_; }

bool Bourgeon::Initialize() {
  LogInfo("Bourgeon {}\n", BOURGEON_VERSION);

  if (!client_.Initialize()) {
    LogError("Bourgeon failed to initialize");
    return false;
  }

  LogInfo("Bourgeon initialized successfully!");
  LoadPlugins("./plugins");

  return true;
}

void Bourgeon::OnTick() {
  // Only run once every 100ms (6 frames at 60fps)
  const auto current_tick_count = GetTickCount();
  if (current_tick_count >= last_tick_count_ &&
      current_tick_count <= last_tick_count_ + 100) {
    return;
  }
  last_tick_count_ = current_tick_count;

  for (auto& registree : callbacks_["OnTick"]) {
    try {
      registree();
    } catch (pybind11::error_already_set& error) {
      LogError(error.what());
      UnregisterCallback("OnTick", registree);
    }
  }
}

void Bourgeon::RegisterCallback(const std::string& callback_name,
                                const pybind11::object& function) {
  try {
    LogInfo("{} has been registered to {}",
            function.attr("__name__").cast<std::string>(), callback_name);
  } catch (pybind11::error_already_set& error) {
    LogError("{}", error.what());
  }
  callbacks_[callback_name].push_back(function);
}

void Bourgeon::UnregisterCallback(const std::string& callback_name,
                                  const pybind11::object& function) {
  for (auto it = callbacks_[callback_name].begin();
       it != callbacks_[callback_name].end(); ++it) {
    if (function.ptr() == it->ptr()) {
      callbacks_[callback_name].erase(it);
      LogInfo("{} has been unregistered from {}",
              function.attr("__name__").cast<std::string>(), callback_name);
      break;
    }
  }
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

  if (h_find == INVALID_HANDLE_VALUE) {
    return;
  }

  do {
    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      continue;
    }

    std::string filename(fd.cFileName);
    LogInfo("Loading {}", filename);
    try {
      eval_file(folder + '/' + filename,
                module::import("__main__").attr("__dict__"));
    } catch (error_already_set& error) {
      LogError("{}", error.what());
    }
  } while (FindNextFileA(h_find, &fd));

  FindClose(h_find);
}
