#pragma once
#include <ctll.hpp>
#include <ctre.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

// CREDITS TO:
// https://github.com/heavyeyelid/virthttp/blob/2877d4867f18f3bc17f5c1c84e37003cb4afbe56/include/virt_wrap/utility.hpp#L41
//
template <typename T, typename V, size_t... I>
void visit_impl(T &&t, V &&v, std::index_sequence<I...>) {
    (..., v(std::get<I>(t)));
}

template <typename T, typename V> void visit(T &&t, V &&v) {
    visit_impl(std::forward<T>(t), std::forward<V>(v),
               std::make_index_sequence<
                   std::tuple_size<typename std::decay<T>::type>::value>());
}
//

// Declare your CTRE re's here
static constexpr auto obj_match = ctll::fixed_string{
    R"((?:([\d.]*) ([\d.]*) ([\d.]*) rg\n)|(?:BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\(([^\n]*)\)Tj\nET))"};

static constexpr auto obj_match2 = ctll::fixed_string{
    R"((?:([\d.]*) ([\d.]*) ([\d.]*) rg\n)|(?:BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\(([^\n]*)\)Tj\nET))"};

// Add to map here
class ReAccel {

    static constexpr std::tuple ctre_regexs = {
        std::pair{
            "page_",
            [](std::string_view sv) { return ctre::range<obj_match>(sv); }},
        std::pair{"page_1", [](std::string_view sv) {
                      return ctre::range<obj_match2>(sv);
                  }}};

    template <class Match, std::size_t... Is>
    static auto match2vec_impl(Match match, std::index_sequence<Is...>) {
        std::vector<std::string_view> ret;
        (ret.push_back(match.template get<Is>()), ...);
        return ret;
    }

    template <class Match>
    static auto match2vec(Match match) -> std::vector<std::string_view> {
        return match2vec_impl(
            match, std::make_index_sequence<std::tuple_size_v<Match>>{});
    }

  public:
    constexpr auto operator[](std::string_view key) const noexcept {
        return [&, key](std::string_view sv) {
            bool done = false;
            std::optional<std::vector<std::vector<std::string_view>>> ret{};
            visit(ctre_regexs, [&](const auto &e) {
                if (done)
                    return;
                if (e.first == key) {
                    const auto range = e.second(sv);
                    if (range.begin() != range.end()) {
                        auto &res = ret.emplace();
                        for (const auto &m : range)
                            res.push_back(match2vec(m));
                    }
                    done = true;
                }
            });
            return ret;
        };
    }
} constexpr regex_accel{};