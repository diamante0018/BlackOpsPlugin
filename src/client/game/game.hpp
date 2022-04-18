#pragma once

#define SELECT(mp, zm) (game::environment::t5mp() ? mp : zm)

namespace game {
enum gamemode { none, multiplayer, zombies };

extern gamemode current;

namespace environment {
bool t5mp();
bool t5zm();
} // namespace environment

template <typename T> class symbol {
public:
  symbol(const size_t t5mp, const size_t t5zm)
      : t5mp_(reinterpret_cast<T*>(t5mp)), t5zm_(reinterpret_cast<T*>(t5zm)) {}

  T* get() const {
    if (environment::t5mp()) {

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

} // namespace game

#include "symbols.hpp"
