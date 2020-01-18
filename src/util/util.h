#ifndef _SRU_UTIL_
#define _SRU_UTIL_
#include <optional>

namespace sru::util {
class Cordinate {
    double x;
    double y;

  public:
    constexpr Cordinate(double x, double y) : x{x}, y{y} {};
    const double &getX() const;
    const double &getY() const;
};
class Color {

  public:
    double r;
    double g;
    double b;
    constexpr Color(double r, double g, double b) : r{r}, g{g}, b{b} {};
};
std::optional<std::string> QFileRead(std::string path);
};     // namespace sru::util
#endif // !_SRU_UTIL_
#include <optional>
