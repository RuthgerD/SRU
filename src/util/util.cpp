#include "util.h"
#include "re_accel.h"
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <type_traits>

namespace sru::util {
const float& Cordinate::getX() const { return x; }
const float& Cordinate::getY() const { return y; }
std::optional<std::string> QFileRead(std::filesystem::path path) {
    if (auto f = std::fopen(path.lexically_normal().c_str(), "r")) {
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
Cordinate::Cordinate(float x, float y) : x{x}, y{y} {}
const std::string Color::toString() const { return std::to_string(r) + ' ' + std::to_string(g) + ' ' + std::to_string(b) + ' ' + "rg"; }
Color::Color(float r, float g, float b) : r{r}, g{g}, b{b} {}
const int cmd(std::string command) {
    // std::cout << "RUNNING COMMAND: " << command << "\n";
    std::system(command.c_str());
    // std::cout << "---\n";
    return 0;
}
std::optional<std::vector<std::vector<std::string>>> re_search(const std::string re, const std::string& data) {
    std::optional<std::vector<std::vector<std::string>>> ret{};
    if (auto acceled = regex_accel[re](data)) {
        ret = acceled;
    }
    if (!ret) {
        // std::cout << "Falling back to runtime re for: " << re << "\n";
        boost::regex expr{re};
        auto& res = ret.emplace();
        std::transform(boost::sregex_iterator(data.begin(), data.end(), expr), boost::sregex_iterator(), std::back_inserter(res), [&](const auto& x) {
            std::vector<std::string> tmp;
            for (auto y : x) {
                tmp.push_back(y.str());
            }
            return tmp;
        });
    }
    if (ret.value().size() > 0) {
        return ret;
    }
    return {};
}
}; // namespace sru::util
