#pragma once

namespace pal {
namespace trait {

template <typename T, T val>
struct integral_constant {
    static constexpr T value = val;
    constexpr operator T() const {
        return value;
    }
};

} // trait
} // pal

