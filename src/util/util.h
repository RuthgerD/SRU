#pragma once

#include <filesystem>
#include <optional>

namespace sru::util {
class Cordinate {
    float x, y;

  public:
    constexpr Cordinate(float x, float y) : x{x}, y{y} {};
    const float &getX() const;
    const float &getY() const;
};
struct Color {
    float r, g, b;
    constexpr Color(float r, float g, float b) : r{r}, g{g}, b{b} {};
    const std::string toString() const;
};
std::optional<std::string> QFileRead(std::filesystem::path path);
const int cmd(std::string command);
int svoi(std::string_view sv);
float svof(std::string_view sv);
double svod(std::string_view sv);
}; // namespace sru::util
