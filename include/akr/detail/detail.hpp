#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace akr::detail {
class debugger final {
  public:
    template <typename... Args>
    debugger(Args&&...) noexcept = delete;

    static void set(std::function<void(std::string_view)>&& fn) noexcept { fn_ = std::move(fn); }

    static void set_default() noexcept { is_default_ = true; }

    static void reset() noexcept {
        fn_         = {};
        is_default_ = false;
    }

    static void notify(const std::string_view msg) noexcept {
        if (fn_) {
            fn_(msg);
        } else if (is_default_) {
            std::cerr << msg << '\n';
        }
    }

  private:
    static inline auto           fn_         = std::function<void(std::string_view)>{};  // NOLINT
    static inline constinit auto is_default_ = false;
};

enum class error_category : std::uint8_t { runtime, logic };

struct error_code final {
    error_category category;
    std::string    msg;
};

class error_impl final {
  public:
    error_impl(const error_category category, const std::string_view msg) noexcept {
        codes_.emplace_back(category, std::string{msg});
    }

    [[nodiscard]] auto what() const noexcept -> std::string {
        auto out = std::string{};
        for (const auto& [i, code] : std::views::enumerate(codes_)) {
            if (i != 0) out += "-> ";

            if (code.category == error_category::logic)
                out += "[Logic Error]: ";
            else if (code.category == error_category::runtime)
                out += "[Runtime Error]: ";

            out += code.msg;
            out += '\n';
        }

        return out;
    }

    [[noreturn]] void panic() const noexcept {
        std::cerr << what();
        std::terminate();
    }

    [[nodiscard]] auto rethrow(const error_category category, const std::string_view msg)
        const noexcept -> error_impl {
        auto out = codes_;
        out.emplace_back(category, std::string{msg});
        return error_impl{std::move(out)};
    }

  private:
    explicit error_impl(std::vector<error_code>&& codes) noexcept : codes_{std::move(codes)} {}

    std::vector<error_code> codes_;
};
}  // namespace akr::detail