#include "regex.h"

namespace sru::re {
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
        std::cout << "using re2 for: " << re << std::endl;
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
auto re_group_count(const std::string& regex) -> size_t {
    const re2::RE2 re{regex};
    if (!re.ok()) {
        return -1;
    }
    return re.NumberOfCapturingGroups();
}
} // namespace sru::re