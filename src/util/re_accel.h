#pragma once
#include <ctll.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <re2/re2.h>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

namespace sru::re {
// CREDITS TO:
// https://github.com/heavyeyelid/virthttp/blob/2877d4867f18f3bc17f5c1c84e37003cb4afbe56/include/virt_wrap/utility.hpp#L41
//
template <typename T, typename V, size_t... I> void visit_impl(T&& t, V&& v, std::index_sequence<I...>) { (..., v(std::get<I>(t))); }

template <typename T, typename V> void visit(T&& t, V&& v) {
    visit_impl(std::forward<T>(t), std::forward<V>(v), std::make_index_sequence<std::tuple_size<typename std::decay<T>::type>::value>());
}
//

// Declare your CTRE re's here
static constexpr auto default_obj_match_key =
    R"((?:([\d.]*) ([\d.]*) ([\d.]*) rg\n)|(?:BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\(([^\n]*)\)Tj\nET))";
static constexpr auto default_obj_match =
    ctll::fixed_string{R"((?:([\d.]*) ([\d.]*) ([\d.]*) rg\n)|(?:BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\(([^\n]*)\)Tj\nET))"};

static constexpr auto PAGE_EXAMPLE_obj_match_key =
    R"((?:([\d.]*) ([\d.]*) ([\d.]*) rg\n)|(?:BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Page [0-9]*)\)Tj\nET))";
static constexpr auto PAGE_EXAMPLE_obj_match = ctll::fixed_string{
    R"((?:([\d.]*) ([\d.]*) ([\d.]*) rg\n)|(?:BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Page [0-9]*)\)Tj\nET))"};

static constexpr auto page_match_key = R"(([0-9]*?) . obj\n<< /BBox.*?>>\nstream\n((\n|.)*?)\nendstream\nendobj)";
static constexpr auto page_match = ctll::fixed_string{R"(([0-9]*?) . obj\n<< /BBox.*?>>\nstream\n((\n|.)*?)\nendstream\nendobj)"};

static constexpr auto page_1_id_key = R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Page 1)\)Tj\nET)";
static constexpr auto page_1_id = ctll::fixed_string{R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Page 1)\)Tj\nET)"};

static constexpr auto page_any_id_key = R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Page [0-9]*)\)Tj\nET)";
static constexpr auto page_any_id = ctll::fixed_string{R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Page [0-9]*)\)Tj\nET)"};

static constexpr auto page_example_id_key = R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Example No.: [0-9]*)\)Tj\nET)";
static constexpr auto page_example_id = ctll::fixed_string{R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Example No.: [0-9]*)\)Tj\nET)"};

static constexpr auto page_example_obj_key = R"((?:([\d.]*) ([\d.]*) ([\d.]*) rg\n)|(?:BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Example No.: [0-9]*|Page [0-9]*)\)Tj\nET))";
static constexpr auto page_example_obj_id = ctll::fixed_string{R"((?:([\d.]*) ([\d.]*) ([\d.]*) rg\n)|(?:BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Example No.: [0-9]*|Page [0-9]*)\)Tj\nET))"};

class ReAccel {

    template <class Match, std::size_t... Is> static auto match2vec_impl(Match match, std::index_sequence<Is...>) {
        std::vector<std::string_view> ret;
        (ret.push_back(match.template get<Is>().to_view()), ...);
        return ret;
    }

    template <class Match> static auto match2vec(Match match) -> std::vector<std::string_view> {
        return match2vec_impl(match, std::make_index_sequence<std::tuple_size_v<Match>>{});
    }
    using OptMatchResults = std::optional<std::vector<std::vector<std::string_view>>>;
    static auto getAllMatches(std::string_view key, std::string_view sv, int limit = -1) -> OptMatchResults;

  public:
    constexpr auto operator[](std::string_view key) const noexcept {
        return [key](const std::string_view sv, int matches = -1) { return getAllMatches(key, sv, matches); };
    };

} constexpr regex_accel{};
} // namespace sru::re
