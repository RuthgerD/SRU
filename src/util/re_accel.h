#pragma once
#include <ctll.hpp>
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
    R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\(([^\n]*)\)Tj\nET)";
static constexpr auto default_obj_match =
    ctll::fixed_string{R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\(([^\n]*)\)Tj\nET)"};

static constexpr auto PAGE_EXAMPLE_obj_match_key =
    R"(\((Page [0-9]*)\)Tj\nET)";
static constexpr auto PAGE_EXAMPLE_obj_match = ctll::fixed_string{
    R"(\((Page [0-9]*)\)Tj\nET)"};

static constexpr auto page_match_key = R"(BBox [^\n]*?\nstream\n((?:.|\n)*?)endstream)";
static constexpr auto page_match = ctll::fixed_string{R"(BBox [^\n]*?\nstream\n((?:.|\n)*?)endstream)"};

static constexpr auto page_1_id_key = R"(\((Page 1)\)Tj)";
static constexpr auto page_1_id = ctll::fixed_string{R"(\((Page 1)\)Tj)"};

static constexpr auto page_any_id_key = R"(\((Page [0-9]*)\)Tj)";
static constexpr auto page_any_id = ctll::fixed_string{R"(\((Page [0-9]*)\)Tj)"};

static constexpr auto page_example_id_key = R"(\((Example No.: [0-9]*)\)Tj)";
static constexpr auto page_example_id = ctll::fixed_string{R"(\((Example No.: [0-9]*)\)Tj)"};

static constexpr auto page_example_obj_key = R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Example No.: [0-9]*|Page [0-9]*)\)Tj\nET)";
static constexpr auto page_example_obj_id = ctll::fixed_string{R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Example No.: [0-9]*|Page [0-9]*)\)Tj\nET)"};

static constexpr auto page_1_plus_key = R"(\((Page 2)\)Tj)";
static constexpr auto page_1_plus = ctll::fixed_string{R"(\((Page 2)\)Tj)"};

static constexpr auto arrhythmia_key = R"(\((Numerical overview of the arrhythmia)\)Tj)";
static constexpr auto arrhythmia = ctll::fixed_string{R"(\((Numerical overview of the arrhythmia)\)Tj)"};

static constexpr auto freq_key = R"(\((Frequency analysis and ST analysis)\)Tj)";
static constexpr auto freq = ctll::fixed_string{R"(\((Frequency analysis and ST analysis)\)Tj)"};

static constexpr auto r0_key = R"(Page [0-9]*)";
static constexpr auto r0 = ctll::fixed_string{R"(Page [0-9]*)"};

static constexpr auto r1_key = R"((.*))";
static constexpr auto r1 = ctll::fixed_string{R"((.*))"};

static constexpr auto r2_key = R"((Bradycardias:)|(No bradycardia found))";
static constexpr auto r2 = ctll::fixed_string{R"((Bradycardias:)|(No bradycardia found))"};

static constexpr auto r3_key = R"((Tachycardias:)|(No tachycardia found))";
static constexpr auto r3 = ctll::fixed_string{R"((Tachycardias:)|(No tachycardia found))"};

static constexpr auto r4_key = R"((Pauses:)|(No pauses found))";
static constexpr auto r4 = ctll::fixed_string{R"((Pauses:)|(No pauses found))"};

static constexpr auto r5_key = R"(Example No.: (.*))";
static constexpr auto r5 = ctll::fixed_string{R"(Example No.: (.*))"};

static constexpr auto r6_key = R"(Page ([0-9]*))";
static constexpr auto r6 = ctll::fixed_string{R"(Page ([0-9]*))"};

static constexpr auto r7_key = R"(Example No.: ([0-9]*))";
static constexpr auto r7 = ctll::fixed_string{R"(Example No.: ([0-9]*))"};

static constexpr auto r8_key = R"(([0-9]+))";
static constexpr auto r8 = ctll::fixed_string{R"(([0-9]+))"};

static constexpr auto r9_key = R"((?:[0-9]*).*?\((.*)%)";
static constexpr auto r9 = ctll::fixed_string{R"((?:[0-9]*).*?\((.*)%)"};

static constexpr auto r10_key = R"(([0-9]*).*?\((?:.*)%)";
static constexpr auto r10 = ctll::fixed_string{R"(([0-9]*).*?\((?:.*)%)"};

static constexpr auto r11_key = R"((.*?)s, .*\. (.*?)/min)";
static constexpr auto r11 = ctll::fixed_string{R"((.*?)s, .*\. (.*?)/min)"};

static constexpr auto r12_key = R"(.*\((.*)%.*)";
static constexpr auto r12 = ctll::fixed_string{R"(.*\((.*)%.*)"};

static constexpr auto r13_key = R"(([0-9]+) episode.?)";
static constexpr auto r13 = ctll::fixed_string{R"(([0-9]+) episode.?)"};

static constexpr auto r14_key = R"(([0-9]+):([0-9]+):([0-9]+))";
static constexpr auto r14 = ctll::fixed_string{R"(([0-9]+):([0-9]+):([0-9]+))"};

static constexpr auto r15_key = R"(([0-9]{2}).([0-9]{2}).([0-9]{4}) ([0-9]{2}):([0-9]{2}):([0-9]{2}))";
static constexpr auto r15 = ctll::fixed_string{R"(([0-9]{2}).([0-9]{2}).([0-9]{4}) ([0-9]{2}):([0-9]{2}):([0-9]{2}))"};

static constexpr auto r16_key = R"(([0-9]+?)/min)";
static constexpr auto r16 = ctll::fixed_string{R"(([0-9]+?)/min)"};

static constexpr auto r17_key = R"((.*)%)";
static constexpr auto r17 = ctll::fixed_string{R"((.*)%)"};

static constexpr auto r18_key = R"(([0-9]+) QRS)";
static constexpr auto r18 = ctll::fixed_string{R"(([0-9]+) QRS)"};

static constexpr auto r19_key = R"((.*?)s.*)";
static constexpr auto r19 = ctll::fixed_string{R"((.*?)s.*)"};

static constexpr auto r20_key = R"((.*)% disturbed)";
static constexpr auto r20 = ctll::fixed_string{R"((.*)% disturbed)"};

static constexpr auto r21_key = R"(Signal quality:)";
static constexpr auto r21 = ctll::fixed_string{R"(Signal quality:)"};

static constexpr auto r22_key = R"(Events:)";
static constexpr auto r22 = ctll::fixed_string{R"(Events:)"};

static constexpr auto r23_key = R"(Duration:)";
static constexpr auto r23 = ctll::fixed_string{R"(Duration:)"};

static constexpr auto r24_key = R"(End:)";
static constexpr auto r24 = ctll::fixed_string{R"(End:)"};

static constexpr auto r25_key = R"(Start:)";
static constexpr auto r25 = ctll::fixed_string{R"(Start:)"};

static constexpr auto r26_key = R"(Middle resting:)";
static constexpr auto r26 = ctll::fixed_string{R"(Middle resting:)"};

static constexpr auto r27_key = R"(Middle active:)";
static constexpr auto r27 = ctll::fixed_string{R"(Middle active:)"};

static constexpr auto r28_key = R"(Average:)";
static constexpr auto r28 = ctll::fixed_string{R"(Average:)"};

static constexpr auto r29_key = R"(Max:)";
static constexpr auto r29 = ctll::fixed_string{R"(Max:)"};

static constexpr auto r30_key = R"(Min:)";
static constexpr auto r30 = ctll::fixed_string{R"(Min:)"};

static constexpr auto r31_key = R"((Salvos / VTA:)|(No salvos / VTA found))";
static constexpr auto r31 = ctll::fixed_string{R"((Salvos / VTA:)|(No salvos / VTA found))"};

static constexpr auto r32_key = R"((Triplets:)|(No triplets found))";
static constexpr auto r32 = ctll::fixed_string{R"((Triplets:)|(No triplets found))"};

static constexpr auto r33_key = R"((Couplets:)|(No couplets found))";
static constexpr auto r33 = ctll::fixed_string{R"((Couplets:)|(No couplets found))"};

static constexpr auto r34_key = R"(VPB per 1000 events:)";
static constexpr auto r34 = ctll::fixed_string{R"(VPB per 1000 events:)"};

static constexpr auto r35_key = R"(VPB per hour:)";
static constexpr auto r35 = ctll::fixed_string{R"(VPB per hour:)"};

static constexpr auto r36_key = R"(VPB in % of events:)";
static constexpr auto r36 = ctll::fixed_string{R"(VPB in % of events:)"};

static constexpr auto r37_key = R"(Number of VPB:)";
static constexpr auto r37 = ctll::fixed_string{R"(Number of VPB:)"};

static constexpr auto r38_key = R"(Number of SVPB:)";
static constexpr auto r38 = ctll::fixed_string{R"(Number of SVPB:)"};

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
