#pragma once

#include <expected>
#include <iostream>

#include "akr/akr.hpp"

[[nodiscard]] inline auto fail1() noexcept -> std::expected<void, akr::error> {
    return akr::throw_logic("Fail1.");
}

[[nodiscard]] inline auto fail2() noexcept -> std::expected<void, akr::error> {
    return fail1().error().rethrow_runtime("Fail.");
}

inline void test1() noexcept {
    std::cerr << fail1().error().what();
}

inline void test2() noexcept {
    std::cerr << fail2().error().what();
}