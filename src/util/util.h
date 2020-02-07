#pragma once

#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace sru::util {
class Coordinate {
    float x, y;

  public:
    // TODO: contexpr
    Coordinate(float x, float y);
    [[nodiscard]] auto getX() -> float&;
    [[nodiscard]] auto getX() const noexcept -> float;
    [[nodiscard]] auto getY()  -> float&;
    [[nodiscard]] auto getY() const noexcept -> float;
};
struct Color {
    float r, g, b;
    Color(float r, float g, float b);
    [[nodiscard]] auto toString() const -> const std::string;
};
auto QFileRead(const std::filesystem::path& path) -> std::optional<std::string>;
auto cmd(std::string command) -> int;
auto re_search(const std::string& re, std::string_view data) -> std::optional<std::vector<std::vector<std::string_view>>>;
auto re_match(const std::string& re, std::string_view data) -> bool;
auto re_replace(const std::string& re, std::string_view repl, std::string& data) -> bool;
auto multi_search(std::string re, const std::vector<std::string>& content, std::vector<int> order)
    -> std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>>;
auto multi_add(std::vector<std::vector<float>> values, int overflow = 1) -> std::vector<float>;
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
// TODO: unsafe :P
auto svto(const std::string_view& sv) -> T {
    auto value = boost::convert<T>(sv, boost::cnv::strtol());
    return value.get();
}
}; // namespace sru::util
