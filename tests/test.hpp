#pragma once

#include <expected>
#include <print>

#include "akr/akr.hpp"

namespace akr::test {
[[nodiscard]] inline auto fail_heap() noexcept -> std::expected<void, error> {
    return make_error(logic_error, "Fail1.");
}

[[nodiscard]] inline auto fail_stack1() noexcept -> std::expected<void, stack_error<1>> {
    return make_stack_error(logic_error, "Fail1.");
}

[[nodiscard]] inline auto fail_stack2() noexcept -> std::expected<void, stack_error<2>> {
    return make_stack_error(runtime_error, "Fail2", fail_stack1().error());
}

[[nodiscard]] inline auto fail_stack3_a() noexcept -> std::expected<void, stack_error<3>> {
    return make_stack_error(runtime_error, "Fail3", fail_stack2().error());
}

[[nodiscard]] inline auto fail_stack3_b() noexcept -> std::expected<void, stack_error<3>> {
    return make_stack_error(runtime_error, "Fail2", fail_stack1().error());
}

[[nodiscard]] inline auto fail_heap2() noexcept -> std::expected<void, error> {
    return make_error(runtime_error, "Fail", fail_stack2().error());
}

inline void test1() noexcept {
    std::println("Test1 start.");
    auto e = fail_heap().error();
    e.cerr();

    std::println("Test1 end.");
}

inline void test2() noexcept {
    std::println("Test2 start.");
    auto e = fail_stack1().error();
    e.cerr();

    std::println("Test2 end.");
}

inline void test3() noexcept {
    std::println("Test3 start.");
    auto e = fail_stack2().error();
    e.cerr();

    std::println("Test3 end.");
}

inline void test4() noexcept {
    std::println("Test4 start.");
    auto e = fail_stack3_a().error();
    e.cerr();

    std::println("Test4 end.");
}

inline void test5() noexcept {
    std::println("Test5 start.");
    auto e = fail_stack3_b().error();
    e.cerr();

    std::println("Test5 end.");
}

inline void test6() noexcept {
    std::println("Test6 start.");
    auto e = fail_heap2().error();
    e.cerr();
    std::println("Test6 end.");
}
}  // namespace akr::test