#ifndef BOURGEON_UTILS_HOOKING_HOOK_MANAGER_H_
#define BOURGEON_UTILS_HOOKING_HOOK_MANAGER_H_

#include <unordered_map>
#include <vector>

namespace hooking {

enum class HookType { kJmpHook = 0, kHwbpHook };

struct HookInfo {
  HookType hookType;
  uint8_t* destination;
  uint8_t* original;
};

class HookManager {
 public:
  // Singleton stuff
  static HookManager& GetInstance() {
    static HookManager instance;
    return instance;
  }
  HookManager(HookManager const&) = delete;
  void operator=(HookManager const&) = delete;

  const std::unordered_map<uint8_t*, HookInfo*>& hook_map() const;
  const std::vector<uint8_t*>& hwbp_hooks() const;

  void* SetHook(HookType, uint8_t*, uint8_t*);
  bool UnsetHook(uint8_t*);

 private:
  HookManager();
  uint8_t* SetJmpHook(uint8_t*, uint8_t*);
  uint8_t* SetHwbpHook(uint8_t*, uint8_t*);
  bool UnsetJmpHook(uint8_t*, uint8_t*);
  bool UnsetHwbpHook(uint8_t*, uint8_t*);
  bool UpdateThreadsContexts();

 private:
  std::unordered_map<uint8_t*, HookInfo*> hook_map_;
  std::vector<uint8_t*> hwbp_hooks_;
  bool handler_added_;
};

}  // namespace hooking
#endif /* BOURGEON_UTILS_HOOKING_HOOK_MANAGER_H_ */