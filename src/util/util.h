#pragma once

#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace sru::util {
class Cordinate {
    float x, y;

  public:
    Cordinate(float x, float y);
    const float& getX() const;
    const float& getY() const;
};
struct Color {
    float r, g, b;
    Color(float r, float g, float b);
    const std::string toString() const;
};
std::optional<std::string> QFileRead(std::filesystem::path path);
const int cmd(std::string command);
std::optional<std::vector<std::vector<std::string_view>>> re_search(const std::string re, const std::string_view data);
template <typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
// TODO: unsafe :P
T svto(const std::string_view& sv) {
    auto value = boost::convert<T>(sv, boost::cnv::strtol());
    return value.get();
}
}; // namespace sru::util
