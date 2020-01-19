#pragma once

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
};
std::optional<std::string> QFileRead(std::string path);
}; // namespace sru::util
