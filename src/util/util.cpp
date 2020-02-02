#include "util.h"
#include "re_accel.h"
#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>
#include <charconv>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>

namespace sru::util {
const float &Cordinate::getX() const { return x; }
const float &Cordinate::getY() const { return y; }
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
const std::string Color::toString() const {
    return std::to_string(r) + ' ' + std::to_string(g) + ' ' +
           std::to_string(b) + ' ' + "rg";
}
const int cmd(std::string command) {
    std::cout << "RUNNING COMMAND: " << command << "\n";
    std::system(command.c_str());
    std::cout << "---\n";
    return 0;
}
// TODO: TEMPLATES
int svoi(std::string_view sv) {
    int result = 0;

    std::from_chars(sv.data(), sv.data() + sv.size(), result);

    return result;
}
float svof(std::string_view sv) {
    float result = 0;

    auto value = boost::convert<float>(sv, boost::cnv::strtol());
    if (value.has_value()) {
        result = value.get();
    }

    return result;
}
double svod(std::string_view sv) {
    double result = 0;
    auto value = boost::convert<double>(sv, boost::cnv::strtol());
    if (value.has_value()) {
        result = value.get();
    }
    return result;
}
std::optional<std::vector<std::vector<std::string>>>
re_search(const std::string re, const std::string &data) {
    std::optional<std::vector<std::vector<std::string>>> ret{};
    if (auto acceled = regex_accel[re](data)) {
        ret = acceled;
    }
    if (!ret) {
        // std::cout << "Falling back to runtime re for: " << re << "\n";
        boost::regex expr{re};
        auto &res = ret.emplace();
        std::transform(boost::sregex_iterator(data.begin(), data.end(), expr),
                       boost::sregex_iterator(), std::back_inserter(res),
                       [&](const auto &x) {
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
