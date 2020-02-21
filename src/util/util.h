#pragma once
#include "regex.h"
#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/range/combine.hpp>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>
#include <cmath>
#include <iostream>
#include <fmt/format.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <filesystem>

namespace sru::pdf {
class StringObject;
}

namespace sru::util {
class Coordinate {
    float x_, y_;

  public:
    // TODO: contexpr
    Coordinate(float x, float y);
    [[nodiscard]] auto getX() -> float&;
    [[nodiscard]] auto getX() const noexcept -> float;
    [[nodiscard]] auto getY() -> float&;
    [[nodiscard]] auto getY() const noexcept -> float;
    auto translateX(float amount) -> void;
    auto translateY(float amount) -> void;
};
struct Color {
    float r_, g_, b_;
    Color(float r, float g, float b);
    [[nodiscard]] auto toString() const -> std::string;
};

auto QFileRead(const std::filesystem::path& path) -> std::optional<std::string>;
auto QFileWrite(const std::string& content, const std::filesystem::path& path) -> bool;
auto cmd(const std::string& command) -> int;

auto multi_search(const std::string& re, const std::vector<std::string>& content, std::vector<int> order)
    -> std::pair<std::vector<std::vector<float>>, std::vector<std::vector<float>>>;
auto multi_add(std::vector<std::vector<float>> values, int overflow = 1) -> std::vector<float>;
// TODO: make into a template
auto multi_sort(const std::vector<std::vector<float>>& values, const std::vector<sru::pdf::StringObject>& objects,
                const std::vector<bool>& settings)
    -> std::pair<std::vector<std::vector<float>>, std::vector<sru::pdf::StringObject>>;
auto multi_avrg(const std::vector<float>& values, const std::vector<float>& compare, float multiplier) -> std::vector<float>;
auto multi_re_place(const std::string& regex, std::string& base, std::vector<std::string> content) -> bool;
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
// TODO: unsafe :P
auto svto(const std::string_view& sv) -> T {
    auto value = boost::convert<T>(sv, boost::cnv::strtol());
    return value.get();
}
template< class Key, class T, class Hash, class KeyEqual, class Alloc, class Pred >
auto erase_if(std::unordered_map<Key,T,Hash,KeyEqual,Alloc>& c, Pred pred) -> void {
    for (auto i = c.begin(), last = c.end(); i != last; ) {
        if (pred(i->first, i->second)) {
            i = c.erase(i);
        } else {
            ++i;
        }
    }
}
auto to_string(const float& value, int decimal_point) -> std::string;
auto to_string(const float& value, float cut_off, int decimal_point) -> std::string;
auto strptime(const std::string& value, const std::string& pattern) -> std::optional<std::chrono::system_clock::time_point>;
} // namespace sru::util
