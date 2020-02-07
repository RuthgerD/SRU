#include "util.h"
#include "re_accel.h"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <filesystem>
#include <re2/re2.h>
#include <type_traits>

namespace sru::util {
auto Coordinate::getX() -> float& { return x; }
auto Coordinate::getX() const noexcept -> float { return x; }
auto Coordinate::getY() -> float& { return y; }
auto Coordinate::getY() const noexcept -> float { return y; }
auto QFileRead(const std::filesystem::path& path) -> std::optional<std::string> {
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
Coordinate::Coordinate(float x, float y) : x{x}, y{y} {}
auto Color::toString() const -> const std::string { return std::to_string(r) + ' ' + std::to_string(g) + ' ' + std::to_string(b) + ' ' + "rg"; }
Color::Color(float r, float g, float b) : r{r}, g{g}, b{b} {}
auto cmd(std::string command) -> int {
    // std::cout << "RUNNING COMMAND: " << command << "\n";
    std::system(command.c_str());
    // std::cout << "---\n";
    return 0;
}
auto re2_search(const std::string& pattern, const std::string_view str) -> std::optional<std::vector<std::vector<std::string_view>>> {
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
    re2::StringPiece piece(str.data(), str.length());
    while (re2::RE2::FindAndConsumeN(&piece, re, argument_ptrs.data(), n)) {
        std::vector<std::string_view> tmp;
        std::copy(results.begin(), results.end(), std::back_inserter(tmp));
        res.push_back(std::move(tmp));
    }

    if (!res.empty()) {
        return ret;
    }
    return {};
}
auto re_search(const std::string& re, const std::string_view data) -> std::optional<std::vector<std::vector<std::string_view>>> {
    std::optional<std::vector<std::vector<std::string_view>>> ret{};
    if (auto acceled = regex_accel[re](data); acceled) {
        ret = acceled;
    }
    if (!ret) {
        ret = re2_search(re, data);
    }
    if (!ret->empty()) {
        return ret;
    }
    return {};
}
auto re_match(const std::string& re, const std::string_view data) -> bool { return re_search(re, data).has_value(); }
auto re_replace(const std::string& regex, const std::string_view repl, std::string& data) -> bool {
    bool replaced = false;
    if (false) {
        // TODO: implement acceled implementation
    }
    if (!replaced) {
        const re2::RE2 re{regex};
        if (!re.ok()) {
            return false;
        }
        re2::StringPiece piece(repl.data(), repl.size());
        replaced = re2::RE2::Replace(&data, re, piece);
    }
    return replaced;
}
auto multi_search(std::string re, const std::vector<std::string>& content, std::vector<int> order)
    -> std::pair<std::vector<std::vector<float>>, std::vector<std::vector<int>>> {
    std::vector<std::vector<float>> total_extracted{};
    std::vector<std::vector<int>> total_count{};
    for (const auto& data : content) {
        if (auto result = re_search(re, data); result) {
            std::vector<float> extracted{};
            std::vector<int> count{};

            const auto& f = result->front();
            for (int i = 1; i < f.size(); ++i) {
                extracted.push_back(svto<float>(f[i]));
                count.push_back(1);
            }
            total_extracted.push_back(std::move(extracted));
            total_count.push_back(std::move(count));
        }
    }
    return {total_extracted, total_count};
}
auto multi_add(std::vector<std::vector<float>> values, int overflow) -> std::vector<float> {
    /// FIXME: add bound checks
    std::vector<float> total(values.front().size());
    for (const auto& value : values) {
        auto count = value.size() - 1;
        std::for_each(value.rbegin(), value.rend(), [&](const auto& x) {
            auto& tc = total[count];
            if (tc + x >= overflow && overflow == -1 && count != 0) {
                tc += x - overflow;
                ++total[count - 1];
            } else {
                tc += x;
            }
            --count;
        });
    }
    return total;
}
}; // namespace sru::util
