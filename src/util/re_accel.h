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

static constexpr const char r0_key[] = R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Page [0-9]*)\)Tj\nET)";
static constexpr auto r0 = ctll::fixed_string{r0_key};

static constexpr const char r1_key[] = R"((.*))";
static constexpr auto r1 = ctll::fixed_string{r1_key};

static constexpr const char r2_key[] = R"((Bradycardias:)|(No bradycardia found))";
static constexpr auto r2 = ctll::fixed_string{r2_key};

static constexpr const char r3_key[] = R"((Tachycardias:)|(No tachycardia found))";
static constexpr auto r3 = ctll::fixed_string{r3_key};

static constexpr const char r4_key[] = R"((Pauses:)|(No pauses found))";
static constexpr auto r4 = ctll::fixed_string{r4_key};

static constexpr const char r5_key[] = R"(Example No.: (.*))";
static constexpr auto r5 = ctll::fixed_string{r5_key};

static constexpr const char r6_key[] = R"(Page ([0-9]*))";
static constexpr auto r6 = ctll::fixed_string{r6_key};

static constexpr const char r7_key[] = R"(Example No.: ([0-9]*))";
static constexpr auto r7 = ctll::fixed_string{r7_key};

static constexpr const char r8_key[] = R"(([0-9]+))";
static constexpr auto r8 = ctll::fixed_string{r8_key};

static constexpr const char r9_key[] = R"((?:[0-9]*).*?\((.*)%)";
static constexpr auto r9 = ctll::fixed_string{r9_key};

static constexpr const char r10_key[] = R"(([0-9]*).*?\((?:.*)%)";
static constexpr auto r10 = ctll::fixed_string{r10_key};

static constexpr const char r11_key[] = R"((.*?)s, .*\. (.*?)/min)";
static constexpr auto r11 = ctll::fixed_string{r11_key};

static constexpr const char r12_key[] = R"(.*\((.*)%.*)";
static constexpr auto r12 = ctll::fixed_string{r12_key};

static constexpr const char r13_key[] = R"(([0-9]+) episode.?)";
static constexpr auto r13 = ctll::fixed_string{r13_key};

static constexpr const char r14_key[] = R"(([0-9]+):([0-9]+):([0-9]+))";
static constexpr auto r14 = ctll::fixed_string{r14_key};

static constexpr const char r15_key[] = R"(([0-9]{2}).([0-9]{2}).([0-9]{4}) ([0-9]{2}):([0-9]{2}):([0-9]{2}))";
static constexpr auto r15 = ctll::fixed_string{r15_key};

static constexpr const char r16_key[] = R"(([0-9]+?)/min)";
static constexpr auto r16 = ctll::fixed_string{r16_key};

static constexpr const char r17_key[] = R"((.*)%)";
static constexpr auto r17 = ctll::fixed_string{r17_key};

static constexpr const char r18_key[] = R"(([0-9]+) QRS)";
static constexpr auto r18 = ctll::fixed_string{r18_key};

static constexpr const char r19_key[] = R"((.*?)s.*)";
static constexpr auto r19 = ctll::fixed_string{r19_key};

static constexpr const char r20_key[] = R"((.*)% disturbed)";
static constexpr auto r20 = ctll::fixed_string{r20_key};

static constexpr const char r21_key[] = R"(Signal quality:)";
static constexpr auto r21 = ctll::fixed_string{r21_key};

static constexpr const char r22_key[] = R"(Events:)";
static constexpr auto r22 = ctll::fixed_string{r22_key};

static constexpr const char r23_key[] = R"(Duration:)";
static constexpr auto r23 = ctll::fixed_string{r23_key};

static constexpr const char r24_key[] = R"(End:)";
static constexpr auto r24 = ctll::fixed_string{r24_key};

static constexpr const char r25_key[] = R"(Start:)";
static constexpr auto r25 = ctll::fixed_string{r25_key};

static constexpr const char r26_key[] = R"(Middle resting:)";
static constexpr auto r26 = ctll::fixed_string{r26_key};

static constexpr const char r27_key[] = R"(Middle active:)";
static constexpr auto r27 = ctll::fixed_string{r27_key};

static constexpr const char r28_key[] = R"(Average:)";
static constexpr auto r28 = ctll::fixed_string{r28_key};

static constexpr const char r29_key[] = R"(Max:)";
static constexpr auto r29 = ctll::fixed_string{r29_key};

static constexpr const char r30_key[] = R"(Min:)";
static constexpr auto r30 = ctll::fixed_string{r30_key};

static constexpr const char r31_key[] = R"((Salvos / VTA:)|(No salvos / VTA found))";
static constexpr auto r31 = ctll::fixed_string{r31_key};

static constexpr const char r32_key[] = R"((Triplets:)|(No triplets found))";
static constexpr auto r32 = ctll::fixed_string{r32_key};

static constexpr const char r33_key[] = R"((Couplets:)|(No couplets found))";
static constexpr auto r33 = ctll::fixed_string{r33_key};

static constexpr const char r34_key[] = R"(VPB per 1000 events:)";
static constexpr auto r34 = ctll::fixed_string{r34_key};

static constexpr const char r35_key[] = R"(VPB per hour:)";
static constexpr auto r35 = ctll::fixed_string{r35_key};

static constexpr const char r36_key[] = R"(VPB in % of events:)";
static constexpr auto r36 = ctll::fixed_string{r36_key};

static constexpr const char r37_key[] = R"(Number of VPB:)";
static constexpr auto r37 = ctll::fixed_string{r37_key};

static constexpr const char r38_key[] = R"(Number of SVPB:)";
static constexpr auto r38 = ctll::fixed_string{r38_key};

static constexpr const char r39_key[] =
    R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\(([^\n]*)\)Tj\nET)";
static constexpr auto r39 = ctll::fixed_string{r39_key};

static constexpr const char r40_key[] =
    R"(\((Page [0-9]*)\)Tj\nET)";
static constexpr auto r40 = ctll::fixed_string{r40_key};

static constexpr const char r41_key[] = R"(BBox [^\n]*?\nstream\n((?:.|\n)*?)endstream)";
static constexpr auto r41 = ctll::fixed_string{r41_key};

static constexpr const char r42_key[] = R"(\((Page 1)\)Tj)";
static constexpr auto r42 = ctll::fixed_string{r42_key};

static constexpr const char r43_key[] = R"(\((Page [0-9]*)\)Tj)";
static constexpr auto r43 = ctll::fixed_string{r43_key};

static constexpr const char r44_key[] = R"(\((Example No.: [0-9]*)\)Tj)";
static constexpr auto r44 = ctll::fixed_string{r44_key};

static constexpr const char r45_key[] =
    R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Example No.: [0-9]*|Page [0-9]*)\)Tj\nET)";
static constexpr auto r45 = ctll::fixed_string{r45_key};

static constexpr const char r46_key[] = R"(\((Page 2)\)Tj)";
static constexpr auto r46 = ctll::fixed_string{r46_key};

static constexpr const char r47_key[] = R"(\((Numerical overview of the arrhythmia)\)Tj)";
static constexpr auto r47 = ctll::fixed_string{r47_key};

static constexpr const char r48_key[] = R"(\((Frequency analysis and ST analysis)\)Tj)";
static constexpr auto r48 = ctll::fixed_string{r48_key};

static constexpr const char r49_key[] = R"((Bradycardias:)|(No bradycardia found))";
static constexpr auto r49 = ctll::fixed_string{r49_key};

static constexpr const char r50_key[] = R"(Report)";
static constexpr auto r50 = ctll::fixed_string{r50_key};

static constexpr const char r51_key[] = R"(SVPB)";
static constexpr auto r51 = ctll::fixed_string{r51_key};

static constexpr const char r52_key[] = R"(Total:)";
static constexpr auto r52 = ctll::fixed_string{r52_key};

static constexpr const char r53_key[] = R"(Time)";
static constexpr auto r53 = ctll::fixed_string{r53_key};

static constexpr const char r54_key[] = R"(HR average)";
static constexpr auto r54 = ctll::fixed_string{r54_key};

static constexpr const char r55_key[] = R"(Result:)";
static constexpr auto r55 = ctll::fixed_string{r55_key};

static constexpr const char r56_key[] = R"((.*))";
static constexpr auto r56 = ctll::fixed_string{r56_key};

static constexpr const char r57_key[] = R"((.*)/min.*)";
static constexpr auto r57 = ctll::fixed_string{r57_key};
template <const auto&... FSes>
struct FxStrList {};

using RegexList = FxStrList<r0,r1,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,r16,r17,r18,r19,r20,r21,r22,r23,r24,r25,r26,r27,r28,r29,r30,r31,r32,r33,r34,r35,r36,r37,r38,r39,r40,r41,r42,r43,r44,r45,r46,r47,r48,r49,r50,r51,r52,r53,r54,r55,r56,r57>;
using SvList = FxStrList<r0_key,r1_key,r3_key,r4_key,r5_key,r6_key,r7_key,r8_key,r9_key,r10_key,r11_key,r12_key,r13_key,r14_key,r15_key,r16_key,r17_key,r18_key,r19_key,r20_key,r21_key,r22_key,r23_key,r24_key,r25_key,r26_key,r27_key,r28_key,r29_key,r30_key,r31_key,r32_key,r33_key,r34_key,r35_key,r36_key,r37_key,r38_key,r39_key,r40_key,r41_key,r42_key,r43_key,r44_key,r45_key,r46_key,r47_key,r48_key, r49_key,r50_key,r51_key,r52_key,r53_key,r54_key,r55_key,r56_key,r57_key>;

using LType = std::vector<std::vector<std::string_view>>(*)(std::string_view, std::size_t);

class ReAccel {

    using OptMatchResults = std::optional<std::vector<std::vector<std::string_view>>>;
    static auto getAllMatches(std::string_view key, std::string_view sv, int limit = -1) -> OptMatchResults;

  public:
    constexpr auto operator[](std::string_view key) const noexcept {
        return [key](const std::string_view sv, int matches = -1) { return getAllMatches(key, sv, matches); };
    };

} constexpr regex_accel{};
} // namespace sru::re
