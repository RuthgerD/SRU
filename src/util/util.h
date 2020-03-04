#pragma once
#include "fmt.h"
#include "../pdf/global_settings.h"
#include "regex.h"
#include <algorithm>
#include <array>
#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/range/combine.hpp>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace sru::pdf {
class StringObject;
}

namespace sru::util {
class Coordinate {
    float x_, y_;

  public:
    constexpr Coordinate(float x, float y) noexcept : x_{x}, y_{y} {}
    [[nodiscard]] constexpr auto getX() noexcept -> float& { return x_; }
    [[nodiscard]] constexpr auto getX() const noexcept -> const float& { return x_; }
    [[nodiscard]] constexpr auto getY() noexcept -> float& { return y_; }
    [[nodiscard]] constexpr auto getY() const noexcept -> const float& { return y_; }
    constexpr auto translateX(float amount) noexcept -> void { x_ += amount; }
    constexpr auto translateY(float amount) noexcept -> void { y_ += amount; }
};
struct Color {
    float r_, g_, b_;
    constexpr Color(float r, float g, float b) noexcept : r_{r}, g_{g}, b_{b} {}
    [[nodiscard]] auto toString() const -> std::string;
};

auto QFileRead(const std::filesystem::path& path) -> std::optional<std::string>;
auto QFileWrite(const std::string& content, const std::filesystem::path& path) -> bool;
auto cmd(const std::string& command) -> int;

auto multi_search(const std::string& re, const std::vector<std::string>& content, std::vector<int> order)
    -> std::pair<std::vector<std::vector<float>>, std::vector<std::vector<float>>>;
auto multi_add(std::vector<std::vector<float>> values, int overflow = 1) -> std::vector<float>;
// TODO: make into a template
auto multi_sort(const std::vector<std::vector<float>>& values, const std::vector<std::string>& elements, const std::vector<bool>& settings)
    -> std::pair<std::vector<std::vector<float>>, std::vector<std::string>>;
auto multi_avrg(const std::vector<float>& values, const std::vector<float>& compare, float multiplier) -> std::vector<float>;
auto multi_re_place(const std::string& regex, std::string& base, std::vector<std::string> content) -> bool;
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> auto svto(const std::string_view& sv) -> std::optional<T> {
    auto value = boost::convert<T>(sv, boost::cnv::strtol());
    if (value) {
        return *value;
    }
    return {};
}
template <class Key, class T, class Hash, class KeyEqual, class Alloc, class Pred>
auto erase_if(std::unordered_map<Key, T, Hash, KeyEqual, Alloc>& c, Pred pred) -> void {
    for (auto i = c.begin(), last = c.end(); i != last;) {
        if (pred(i->first, i->second)) {
            i = c.erase(i);
        } else {
            ++i;
        }
    }
}
auto zfill(std::string& string, int amount) -> void;
auto to_string(const float& value, int decimal_point, int zfill_amount = 0) -> std::string;
auto to_string(const float& value, float cut_off, int decimal_point, int zfill_amount = 0) -> std::string;
auto strptime(const std::string& value, const std::string& pattern) -> std::optional<std::chrono::system_clock::time_point>;
auto strftime(const std::chrono::system_clock::time_point& value, const std::string& pattern) -> std::optional<std::string>;
auto escape(std::string& in, const char escape, std::string_view needs_escape) -> void;
template <class T> auto sink(T&&) noexcept {}
} // namespace sru::util
