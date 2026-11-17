#pragma once

#include <expected>
#include <functional>
#include <string>
#include <string_view>
#include <utility>

#include "akr/detail/detail.hpp"

namespace akr {
inline void set_debug_fn(std::function<void(std::string_view)> fn) noexcept {
    detail::debugger::set(std::move(fn));
}

inline void use_default_debugger() noexcept {
    detail::debugger::set_default();
}

inline void notify(const std::string_view msg) noexcept {
    detail::debugger::notify(msg);
}

[[nodiscard]] inline auto fail(const std::string_view msg) noexcept -> bool {
    notify(msg);
    return false;
}

class error final {
    friend auto throw_logic(const std::string_view msg) noexcept -> std::unexpected<error>;

    friend auto throw_runtime(const std::string_view msg) noexcept -> std::unexpected<error>;

  public:
    [[nodiscard]] auto what() const noexcept -> std::string { return impl_.what(); }

    [[noreturn]] void panic() const noexcept { impl_.panic(); }

    [[nodiscard]] auto rethrow_logic(const std::string_view msg
    ) const noexcept -> std::unexpected<error> {
        return rethrow(detail::error_category::logic, msg);
    }

    [[nodiscard]] auto rethrow_runtime(const std::string_view msg
    ) const noexcept -> std::unexpected<error> {
        return rethrow(detail::error_category::runtime, msg);
    }

  private:
    error(const detail::error_category category, const std::string_view msg) noexcept
        : impl_{category, msg} {}

    error(detail::error_impl&& impl) noexcept : impl_{std::move(impl)} {}

    [[nodiscard]] auto rethrow(const detail::error_category category, const std::string_view msg)
        const noexcept -> std::unexpected<error> {
        return std::unexpected{error{impl_.rethrow(category, msg)}};
    }

    detail::error_impl impl_;
};

[[nodiscard]] inline auto throw_logic(const std::string_view msg
) noexcept -> std::unexpected<error> {
    return std::unexpected{error{detail::error_category::logic, msg}};
}

[[nodiscard]] inline auto throw_runtime(const std::string_view msg
) noexcept -> std::unexpected<error> {
    return std::unexpected{error{detail::error_category::runtime, msg}};
}
}  // namespace akr