#include "util.h"

namespace sru::util {
const double &Cordinate::getX() const { return x; }
const double &Cordinate::getY() const { return y; }
std::optional<std::string> QFileRead(std::string path) {
    if (auto f = std::fopen(path.data(), "r")) {
        std::fseek(f, 0, SEEK_END);
        std::string str;
        str.resize(std::ftell(f));
        std::fseek(f, 0, SEEK_SET);
        std::fread(str.data(), str.length(), 1, f);
        std::fclose(f);
        return str;
    } else {
        return {};
    }
}
} // namespace sru::util
