#pragma once

// Taken from https://github.com/curio-r/rextensions

template <typename T>
union _void_cast_u {
  T Fn;
  char *Ptr;

  inline _void_cast_u(T fn) : Fn(fn) {}
};

#define void_cast(T) ((_void_cast_u<decltype(T)>{T}).Ptr)

template <typename Class, typename Func>
struct MethodRef {
 private:
  Func _Ptr = nullptr;

 public:
  bool IsCallable() { return _Ptr != nullptr; }

  template <typename Type>
  inline void operator=(Type Ptr) {
    _Ptr = *reinterpret_cast<Func *>(&Ptr);
  }

  template <typename... Types>
  auto operator()(Class *_Context, Types... _Args)
      -> decltype((_Context->*_Ptr)(_Args...)) {
    return (_Context->*_Ptr)(_Args...);
  }
};
