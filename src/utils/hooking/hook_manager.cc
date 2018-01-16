#include "hook_manager.h" =
#include <Windows.h>
#include <stdexcept>
#include "detours.h"

#include <TlHelp32.h>

namespace hooking {

static LONG CALLBACK ExceptionHandler(PEXCEPTION_POINTERS);

HookManager::HookManager()
    : hook_map_(), hwbp_hooks_(), handler_added_(false){};

const std::unordered_map<uint8_t*, HookInfo*>& HookManager::hook_map() const {
  return hook_map_;
}

const std::vector<uint8_t*>& HookManager::hwbp_hooks() const {
  return hwbp_hooks_;
}

void* HookManager::SetHook(HookType hookType, uint8_t* hookAddr,
                           uint8_t* destination) {
  PBYTE original = NULL;
  switch (hookType) {
    case HookType::kJmpHook:
      original = SetJmpHook(hookAddr, destination);
      break;
    case HookType::kHwbpHook:
      original = SetHwbpHook(hookAddr, destination);
      break;
    default:
      return NULL;
  }
  if (NULL == original) return NULL;

  // REGISTER THE HOOK
  HookInfo* hookInfo = new HookInfo;

  hookInfo->hookType = hookType;
  hookInfo->destination = destination;
  hookInfo->original = original;

  hook_map_[hookAddr] = hookInfo;

  return original;
}

bool HookManager::UnsetHook(uint8_t* hookAddr) {
  try {
    HookInfo* hookInfo = hook_map_.at(hookAddr);
    if (NULL == hookInfo) return false;

    bool result;
    switch (hookInfo->hookType) {
      case HookType::kJmpHook:
        result = UnsetJmpHook(hookAddr, hookInfo->original);
        hook_map_[hookAddr] = NULL;
        delete hookInfo;
        return result;
        break;
      case HookType::kHwbpHook:
        result = UnsetHwbpHook(hookAddr, hookInfo->original);
        hook_map_[hookAddr] = NULL;
        delete hookInfo;
        return result;
        break;
      default:
        return false;
    }
  } catch (const std::out_of_range&) {
  }

  return false;
}

//===========================================
//============= PRIVATE METHODS =============
//===========================================

uint8_t* HookManager::SetJmpHook(uint8_t* hookAddr, uint8_t* destination) {
  PBYTE original = NULL;
  MEMORY_BASIC_INFORMATION MBI;

  if (!VirtualQuery(hookAddr, &MBI, sizeof(MBI))) return NULL;

  DWORD oldProtection;

  if (!VirtualProtect(hookAddr, 0x5, PAGE_EXECUTE_READWRITE, &oldProtection))
    return NULL;

  original = (PBYTE)VirtualAlloc(NULL, 23, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  if (NULL == original) return NULL;

  // Copy the original function
  DWORD size = 0;
  while (size < 5) {
    PBYTE next =
        (PBYTE)DetourCopyInstruction(original + size, hookAddr + size, NULL);
    size = next - hookAddr;
  }

  // Put JMP from original+size to hookAddr+size
  INT32 jmpSrc = (INT32)(original + size + 5);
  INT32 jmpDst = (INT32)(hookAddr + size);
  *(original + size) = 0xE9;
  *((PINT32)(original + size + 1)) = (INT32)(jmpDst - jmpSrc);

  // Put JMP from hookAddr to destination
  jmpSrc = (INT32)(hookAddr + 5);
  jmpDst = (INT32)(destination);
  *(hookAddr) = 0xE9;
  *((PINT32)(hookAddr + 1)) = (INT32)(jmpDst - jmpSrc);

  VirtualProtect(hookAddr, 0x5, oldProtection, &oldProtection);
  FlushInstructionCache(GetCurrentProcess(), hookAddr, 0x5);

  return original;
}

uint8_t* HookManager::SetHwbpHook(uint8_t* hookAddr, uint8_t* destination) {
  if (hwbp_hooks_.size() > 3) return NULL;

  if (!handler_added_) {
    if (!AddVectoredExceptionHandler(1, ExceptionHandler)) return NULL;

    handler_added_ = true;
  }
  hwbp_hooks_.push_back(hookAddr);

  PBYTE original =
      (PBYTE)VirtualAlloc(NULL, 14, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  if (NULL == original) return NULL;
  DWORD size = 0;
  // Copy the original function
  PBYTE next = (PBYTE)DetourCopyInstructionEx(original + size, hookAddr + size,
                                              NULL, NULL);
  size = next - hookAddr;

  // Put JMP from original+size to hookAddr+size
  INT32 jmpSrc = (INT32)(original + size + 5);
  INT32 jmpDst = (INT32)(hookAddr + size);
  *(original + size) = 0xE9;
  *((PINT32)(original + size + 1)) = (INT32)(jmpDst - jmpSrc);

  if (!UpdateThreadsContexts()) return NULL;

  return original;
}

bool HookManager::UnsetJmpHook(uint8_t* hookAddr, uint8_t* original) {
  DWORD oldProtection;

  if (!VirtualProtect(hookAddr, 0x5, PAGE_EXECUTE_READWRITE, &oldProtection))
    return false;

  DWORD size = 0;
  while (size < 5) {
    PBYTE next =
        (PBYTE)DetourCopyInstruction(hookAddr + size, original + size, NULL);
    size = next - original;
  }
  FlushInstructionCache(GetCurrentProcess(), hookAddr, 0x5);

  VirtualFree(original, 23, MEM_RELEASE);

  VirtualProtect(hookAddr, 0x5, oldProtection, &oldProtection);
  return true;
}

bool HookManager::UnsetHwbpHook(uint8_t* hookAddr, uint8_t* original) {
  if (hwbp_hooks_.empty()) return false;

  DWORD index = 0;
  while (index < hwbp_hooks_.size()) {
    if (hwbp_hooks_[index] == hookAddr) break;
    index++;
  }
  if (index > 3) return false;

  hwbp_hooks_.pop_back();

  return true;
}

bool HookManager::UpdateThreadsContexts() {
  HANDLE thSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
  THREADENTRY32 te;
  te.dwSize = sizeof(THREADENTRY32);
  CONTEXT ctx;

  DWORD DR0 = 0, DR1 = 0, DR2 = 0, DR3 = 0, DR7 = 0;

  switch (hwbp_hooks_.size()) {
    case 4:
      DR3 = (DWORD)hwbp_hooks_[3];
      DR7 = 0x00000040;
    case 3:
      DR2 = (DWORD)hwbp_hooks_[2];
      DR7 = DR7 | 0x00000010;
    case 2:
      DR1 = (DWORD)hwbp_hooks_[1];
      DR7 = DR7 | 0x00000004;
    case 1:
      DR0 = (DWORD)hwbp_hooks_[0];
      DR7 = DR7 | 0x00000001;
      break;
    default:
      return false;
  }

  Thread32First(thSnap, &te);
  do {
    if (te.th32OwnerProcessID != GetCurrentProcessId()) continue;

    HANDLE hThread = OpenThread(
        THREAD_QUERY_INFORMATION | THREAD_GET_CONTEXT | THREAD_SET_CONTEXT,
        FALSE, te.th32ThreadID);
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(hThread, &ctx)) return false;

    ctx.Dr0 = DR0;
    ctx.Dr1 = DR1;
    ctx.Dr2 = DR2;
    ctx.Dr3 = DR3;
    ctx.Dr7 = DR7;

    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (!SetThreadContext(hThread, &ctx)) return false;
    CloseHandle(hThread);
  } while (Thread32Next(thSnap, &te));

  return true;
}

LONG CALLBACK ExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo) {
  if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP) {
    auto hwbp_hooks = HookManager::GetInstance().hwbp_hooks();
    size_t index = 0;

    while (index < 4) {
      if (hwbp_hooks[index] == (PVOID)ExceptionInfo->ContextRecord->Eip) break;
      index++;
    }
    if (index > 3) return EXCEPTION_CONTINUE_SEARCH;

    auto hook_map = HookManager::GetInstance().hook_map();

    ExceptionInfo->ContextRecord->Eip =
        (DWORD)((hook_map[hwbp_hooks[index]])->destination);
    return EXCEPTION_CONTINUE_EXECUTION;
  }

  return EXCEPTION_CONTINUE_SEARCH;
}

}  // namespace hooking