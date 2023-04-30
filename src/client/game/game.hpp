#pragma once

namespace game {
enum gamemode { none, multiplayer, zombies };

extern gamemode current;

namespace environment {
bool is_mp();
bool is_sp();
} // namespace environment

template <typename T> class symbol {
public:
  symbol(const size_t t5mp, const size_t t5zm)
      : t5mp_(reinterpret_cast<T*>(t5mp)), t5zm_(reinterpret_cast<T*>(t5zm)) {}

  [[nodiscard]] T* get() const {
    if (environment::is_mp()) {
      return t5mp_;
    }

    return t5zm_;
  }

  operator T*() const { return this->get(); }

  T* operator->() const { return this->get(); }

private:
  T* t5mp_;
  T* t5zm_;
};

inline std::size_t select(const std::size_t mp_val, const std::size_t sp_val) {
  return environment::is_mp() ? mp_val : sp_val;
}

inline std::size_t select(const void* mp_val, const void* sp_val) {
  return select(reinterpret_cast<std::size_t>(mp_val),
                reinterpret_cast<std::size_t>(sp_val));
}
} // namespace game

#include "symbols.hpp"
