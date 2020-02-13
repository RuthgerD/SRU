#include "util.h"
#include "../pdf_structures/string_obj.h"
#include "re_accel.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <re2/re2.h>
#include <type_traits>

namespace sru::util {
auto Coordinate::getX() -> float& { return x; }
auto Coordinate::getX() const noexcept -> float { return x; }
auto Coordinate::getY() -> float& { return y; }
auto Coordinate::getY() const noexcept -> float { return y; }
auto Coordinate::translateX(float amount) -> void { x += amount; }
auto Coordinate::translateY(float amount) -> void { y += amount; }
auto QFileRead(const std::filesystem::path& path) -> std::optional<std::string> {
    if (auto f = std::fopen(path.lexically_normal().c_str(), "r"); f) {
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
auto QFileWrite(const std::string& content, const std::filesystem::path& path) -> bool {
    if (std::ofstream out(path.lexically_normal().c_str()); out) {
        out << content;
        out.close();
        return true;
    }
    return false;
}
Coordinate::Coordinate(float x, float y) : x{x}, y{y} {}
auto Color::toString() const -> std::string { return std::to_string(r) + ' ' + std::to_string(g) + ' ' + std::to_string(b) + ' ' + "rg"; }
Color::Color(float r, float g, float b) : r{r}, g{g}, b{b} {}
auto cmd(const std::string& command) -> int {
    // std::cout << "RUNNING COMMAND: " << command << "\n";
    std::system(command.c_str());
    // std::cout << "---\n";
    return 0;
}
auto re2_search(const std::string& pattern, const std::string_view str, int limit = -1) -> std::optional<std::vector<std::vector<std::string_view>>> {
    std::string wrapped_pattern = "(" + pattern + ")";
    const re2::RE2 re{wrapped_pattern};
    if (!re.ok()) {
        return {};
    }
    const std::size_t n = re.NumberOfCapturingGroups();

    std::vector<re2::RE2::Arg> arguments(n);

    std::vector<re2::RE2::Arg*> argument_ptrs(n);

    std::vector<re2::StringPiece> results(n);

    for (std::size_t i = 0; i < n; ++i) {
        arguments[i] = &results[i];
        argument_ptrs[i] = &arguments[i];
    }

    std::optional<std::vector<std::vector<std::string_view>>> ret{};
    auto& res = ret.emplace();
    re2::StringPiece piece(str.data(), str.size());
    int i = 0;
    while (re2::RE2::FindAndConsumeN(&piece, re, argument_ptrs.data(), n) && !(i > limit && limit > 0)) {
        std::vector<std::string_view> tmp;
        std::transform(results.begin(), results.end(), std::back_inserter(tmp), [](const auto& x) { return std::string_view{x.data(), x.size()}; });
        res.push_back(std::move(tmp));
        ++i;
    }

    if (!res.empty()) {
        return ret;
    }
    return {};
}
auto re_search(const std::string& re, const std::string_view data, int limit) -> std::optional<std::vector<std::vector<std::string_view>>> {
    std::optional<std::vector<std::vector<std::string_view>>> ret{};
    if (auto acceled = regex_accel[re](data, limit); acceled) {
        ret = acceled;
    }
    if (!ret) {
        ret = re2_search(re, data, limit);
    }
    if (!ret->empty()) {
        return ret;
    }
    return {};
}
auto re_match(const std::string& re, const std::string_view data) -> bool { return re_search(re, data, 1).has_value(); }
auto re_replace(const std::string& regex, const std::string_view repl, std::string& data) -> bool {
    bool replaced = false;
    if (auto acceled = regex_accel[regex](data); acceled) {
        const auto& views = acceled->front().front();
        data.replace(views.data() - data.data(), views.size(), repl);
        replaced = true;
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
auto re_group_count(const std::string& regex) -> int {
    const re2::RE2 re{regex};
    if (!re.ok()) {
        return -1;
    }
    return re.NumberOfCapturingGroups();
}
auto multi_search(const std::string& re, const std::vector<std::string>& content, std::vector<int> order)
    -> std::pair<std::vector<std::vector<float>>, std::vector<std::vector<float>>> {
    std::vector<std::vector<float>> total_extracted{};
    std::vector<std::vector<float>> total_count{};

    // checks if we can use it
    order.erase(std::unique(order.begin(), order.end()), order.end());
    if (re_group_count(re) != order.size()) {
        std::cout << "1. Invalid order for multi_search";
        return {total_extracted, total_count};
    }
    auto max = *std::max_element(order.begin(), order.end());
    auto min = *std::min_element(order.begin(), order.end());
    if (max > order.size() || min < 0) {
        std::cout << "2. Invalid order for multi_search";
        return {total_extracted, total_count};
    }

    for (const auto& data : content) {
        if (auto result = re_search(re, data); result) {
            const auto& f = result->front();
            std::vector<float> extracted{};
            std::vector<float> count{};
            for (int i = 1; i < f.size(); ++i) {
                extracted.push_back(svto<float>(f[order[i - 1]]));
                count.push_back(1);
            }
            total_extracted.push_back(std::move(extracted));
            total_count.push_back(std::move(count));
        }
    }
    return {total_extracted, total_count};
}
// TODO: add boolean variable to disable overflow instead of checking if its -1
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
auto multi_sort(const std::vector<std::vector<float>>& values, const std::vector<std::reference_wrapper<sru::pdf::StringObject>>& objects,
                const std::vector<bool>& settings)
    -> std::pair<std::vector<std::vector<float>>, std::vector<std::reference_wrapper<sru::pdf::StringObject>>> {

    std::vector<std::pair<std::vector<float>, std::reference_wrapper<sru::pdf::StringObject>>> zip;
    zip.reserve(values.size());
    for (int i = 0; i < values.size(); ++i) {
        zip.emplace_back(values[i], objects[i]);
    }
    std::sort(zip.begin(), zip.end(), [&settings](const auto& a, const auto& b) {
        auto compa = a.first;
        auto compb = b.first;

        int diff = 0;
        for (int i = 0; i < compa.size(); ++i) {
            auto tmp = (compa[i] - compb[i]) * std::pow(10, compa.size() - i);
            if (!settings[i]) {
                tmp *= -1;
            }
            diff += tmp;
        }
        return diff > 0;
    });
    std::vector<std::vector<float>> unzipped1;
    std::vector<std::reference_wrapper<sru::pdf::StringObject>> unzipped2;
    for (auto& i : zip) {
        unzipped1.push_back(i.first);
        unzipped2.push_back(i.second);
    }

    return std::pair{unzipped1, unzipped2};
}
auto multi_avrg(const std::vector<float>& values, const std::vector<float>& compare, float multiplier) -> std::vector<float> {
    std::vector<float> result(values.size());
    for (int i = 0; i < values.size(); ++i) {
        result[i] = (values[i] / compare[i]) * multiplier;
    }
    return result;
}
auto multi_re_place(const std::string& regex, std::string& base, std::vector<std::string> content) -> bool {
    if (re_group_count(regex) != content.size()) {
        std::cout << "warning: multi_re_place: content doesnt match regex groups" << std::endl;
        return false;
    }
    int i = 0;
    for (; i < content.size(); ++i) {
        if (auto tmp = re_search(regex, base); tmp) {
            const auto& views = tmp->front();
            base.replace(views[i + 1].data() - base.data(), views[i + 1].size(), content[i]);
        }
    }
    return (i == content.size());
}
} // namespace sru::util
