#pragma once

#define SESSION_THIS(VERSION)                         \
  (reinterpret_cast<Session_##VERSION::Attributes *>( \
      Session::g_session_ptr.load()))

#define SESSION_IMPLEMENTATION(VERSION, ATTRIBUTES)                    \
  class Session_##VERSION final : public Session {                     \
   public:                                                             \
    Session_##VERSION(const YAML::Node &session_configuration)         \
        : Session(session_configuration) {}                            \
    inline uint32_t Session_##VERSION::aid() const override {          \
      return SESSION_THIS(VERSION)->aid_;                              \
    }                                                                  \
    inline int Session_##VERSION::max_hp() const override {            \
      return SESSION_THIS(VERSION)->max_hp_;                           \
    }                                                                  \
    inline int Session_##VERSION::hp() const override {                \
      return SESSION_THIS(VERSION)->hp_;                               \
    }                                                                  \
    inline int Session_##VERSION::max_sp() const override {            \
      return SESSION_THIS(VERSION)->max_sp_;                           \
    }                                                                  \
    inline int Session_##VERSION::sp() const override {                \
      return SESSION_THIS(VERSION)->sp_;                               \
    }                                                                  \
    inline const char *Session_##VERSION::char_name() const override { \
      return SESSION_THIS(VERSION)->char_name_;                        \
    }                                                                  \
                                                                       \
   protected:                                                          \
    inline const std::list<ItemInfo> &Session_##VERSION::item_list()   \
        const override {                                               \
      return SESSION_THIS(VERSION)->item_list_;                        \
    }                                                                  \
                                                                       \
   private:                                                            \
    struct Attributes ATTRIBUTES;                                      \
  }

using TalkTypeTable = std::vector<std::pair<char const *, TalkType>>;
