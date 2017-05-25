#pragma once

namespace pal {
namespace trait {

/// represent a list of types
template <typename...>
struct list {};

template <typename... T>
constexpr list<T...> make_list(T && ...t) {
    return { std::forward<T>(t)... };
};

} // trait
} // pal

