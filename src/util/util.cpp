#include "util.h"
#include "../pdf/string_obj.h"

namespace sru::util {
auto Coordinate::getX() -> float& { return x_; }
auto Coordinate::getX() const noexcept -> float { return x_; }
auto Coordinate::getY() -> float& { return y_; }
auto Coordinate::getY() const noexcept -> float { return y_; }
auto Coordinate::translateX(float amount) -> void { x_ += amount; }
auto Coordinate::translateY(float amount) -> void { y_ += amount; }
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
Coordinate::Coordinate(float x, float y) : x_{x}, y_{y} {}
auto Color::toString() const -> std::string { return std::to_string(r_) + ' ' + std::to_string(g_) + ' ' + std::to_string(b_) + ' ' + "rg"; }
Color::Color(float r, float g, float b) : r_{r}, g_{g}, b_{b} {}
auto cmd(const std::string& command) -> int {
    // std::cout << "RUNNING COMMAND: " << command << "\n";
    std::system(command.c_str());
    // std::cout << "---\n";
    return 0;
}
auto multi_search(const std::string& re, const std::vector<std::string>& content, std::vector<int> order)
    -> std::pair<std::vector<std::vector<float>>, std::vector<std::vector<float>>> {
    std::vector<std::vector<float>> total_extracted{};
    std::vector<std::vector<float>> total_count{};

    // checks if we can use it
    order.erase(std::unique(order.begin(), order.end()), order.end());
    if (sru::re::re_group_count(re) != order.size()) {
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
        if (auto result = sru::re::re_search(re, data, 1); result) {
            auto& f = result->front();
            std::vector<float> extracted{};
            std::vector<float> count{};
            for (size_t i = 1; i < f.size(); ++i) {
                // TODO: do a better job of cleaning shit
                std::string repl{f[order[i - 1]]};
                std::replace(repl.begin(), repl.end(), ',', '.');
                std::string cleaned{};
                std::string cleaned2{};
                std::remove_copy(repl.begin(), repl.end(),std::back_inserter(cleaned), ' ');
                std::remove_copy(cleaned.begin(), cleaned.end(),std::back_inserter(cleaned2), '<');


                extracted.push_back(svto<float>(cleaned2));
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
            if (tc + x >= overflow && overflow != -1 && count != 0) {
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
auto multi_sort(const std::vector<std::vector<float>>& values, const std::vector<std::string>& elements,
                const std::vector<bool>& settings)
    -> std::pair<std::vector<std::vector<float>>, std::vector<std::string>> {

    std::vector<std::pair<std::vector<float>, std::string>> zip;
    zip.reserve(values.size());
    for (size_t i = 0; i < values.size(); ++i) {
        zip.emplace_back(values[i], elements[i]);
    }
    std::sort(zip.begin(), zip.end(), [&settings](const auto& a, const auto& b) {
        auto compa = a.first;
        auto compb = b.first;

        int diff = 0;
        for (size_t i = 0; i < compa.size(); ++i) {
            auto tmp = (compa[i] - compb[i]) * std::pow(10, compa.size() - i);
            if (!settings[i]) {
                tmp *= -1;
            }
            diff += tmp;
        }
        return diff > 0;
    });
    std::vector<std::vector<float>> unzipped1;
    std::vector<std::string> unzipped2;
    for (auto& i : zip) {
        unzipped1.push_back(i.first);
        unzipped2.push_back(i.second);
    }

    return std::pair{unzipped1, unzipped2};
}
auto multi_avrg(const std::vector<float>& values, const std::vector<float>& compare, float multiplier) -> std::vector<float> {
    std::vector<float> result(values.size());
    for (size_t i = 0; i < values.size(); ++i) {
        result[i] = (values[i] / compare[i]) * multiplier;
    }
    return result;
}
auto multi_re_place(const std::string& regex, std::string& base, std::vector<std::string> content) -> bool {
    if (sru::re::re_group_count(regex) != content.size()) {
        std::cout << "warning: multi_re_place: content doesnt match regex groups" << std::endl;
        return false;
    }
    size_t i = 0;
    for (; i < content.size(); ++i) {
        if (auto tmp = sru::re::re_search(regex, base ,1); tmp) {
            const auto& views = tmp->front();
            base.replace(views[i + 1].data() - base.data(), views[i + 1].size(), content[i]);
        }
    }
    return (i == content.size());
}
// TODO: ABSOLUTELY GARBAGE PLEASE LEARN TO USE FMT PROPERLY
auto to_string(const float& value, int decimal_point) -> std::string {
    int accuracy = std::pow(10, decimal_point + 1);
    float rounded = value + ((5.0 / 9.0) / accuracy);
    auto str = fmt::format("{:." + std::to_string(decimal_point) + "f}", rounded);
    while (str.back() == '0') {
        str.pop_back();
    }
    if (str.back() == '.') {
        str.append("0");
    }
    return str;
}
auto to_string(const float& value, float cut_off, int decimal_point) -> std::string {
    if (value < cut_off) {
        return ("<" + to_string(cut_off, decimal_point));
    }
    return to_string(value, decimal_point);
}
auto strptime(const std::string& value, const std::string& pattern) -> std::optional<std::chrono::system_clock::time_point> {
    std::tm tm = {};
    std::stringstream ss(value);
    ss >> std::get_time(&tm, pattern.data());
    if (ss.fail())
        return {};
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}
} // namespace sru::util
