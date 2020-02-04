#include "util.h"
#include "re_accel.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <re2/re2.h>
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
std::optional<std::vector<std::vector<std::string_view>>> re2_search(const std::string& pattern, const std::string_view str) {
    std::string wrapped_pattern = "(" + pattern + ")";
    const re2::RE2 re{wrapped_pattern};
    if (!re.ok()) {
        return {};
    }
    const std::size_t n = re.NumberOfCapturingGroups();

    std::vector<re2::RE2::Arg> arguments(n);

    std::vector<re2::RE2::Arg*> argument_ptrs(n);

    std::vector<std::string> results(n);

    for (std::size_t i = 0; i < n; ++i) {
        arguments[i] = &results[i];
        argument_ptrs[i] = &arguments[i];
    }

    std::optional<std::vector<std::vector<std::string_view>>> ret{};
    auto& res = ret.emplace();
    re2::StringPiece piece(str);
    while (re2::RE2::FindAndConsumeN(&piece, re, argument_ptrs.data(), n)) {
        std::vector<std::string_view> tmp;
        std::copy(results.begin(), results.end(), std::back_inserter(tmp));
        res.push_back(std::move(tmp));
    }

    if (res.size() > 0) {
        return ret;
    }
    return {};
}
std::optional<std::vector<std::vector<std::string_view>>> re_search(const std::string re, const std::string_view data) {
    std::optional<std::vector<std::vector<std::string_view>>> ret{};
    if (auto acceled = regex_accel[re](data); acceled) {
        ret = acceled;
    }
    if (!ret) {
        ret = re2_search(re, data);
    }
    if (ret->size() > 0) {
        return ret;
    }
    return {};
}
bool re_match(const std::string re, const std::string_view data) { return re_search(re, data).has_value(); }
bool re_replace(const std::string regex, const std::string_view repl, std::string& data) {
    bool replaced = false;
    if (false) {
        // TODO: implement acceled implementation
    }
    if (!replaced) {
        const re2::RE2 re{regex};
        if (!re.ok()) {
            return false;
        }
        replaced = re2::RE2::Replace(&data, re, repl);
    }
    return replaced;
}
}; // namespace sru::util
