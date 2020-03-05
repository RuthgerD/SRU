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
extern std::vector<std::vector<std::string_view>> fc0(std::string_view subject, std::size_t limit);

static constexpr const char r1_key[] = R"((.*))";
extern std::vector<std::vector<std::string_view>> fc1(std::string_view subject, std::size_t limit);

static constexpr const char r2_key[] = R"((Bradycardias:)|(No bradycardia found))";
extern std::vector<std::vector<std::string_view>> fc2(std::string_view subject, std::size_t limit);

static constexpr const char r3_key[] = R"((Tachycardias:)|(No tachycardia found))";
extern std::vector<std::vector<std::string_view>> fc3(std::string_view subject, std::size_t limit);

static constexpr const char r4_key[] = R"((Pauses:)|(No pauses found))";
extern std::vector<std::vector<std::string_view>> fc4(std::string_view subject, std::size_t limit);

static constexpr const char r5_key[] = R"(Example No.: (.*))";
extern std::vector<std::vector<std::string_view>> fc5(std::string_view subject, std::size_t limit);

static constexpr const char r6_key[] = R"(Page ([0-9]*))";
extern std::vector<std::vector<std::string_view>> fc6(std::string_view subject, std::size_t limit);

static constexpr const char r7_key[] = R"(Example No.: ([0-9]*))";
extern std::vector<std::vector<std::string_view>> fc7(std::string_view subject, std::size_t limit);

static constexpr const char r8_key[] = R"(([0-9]+))";
extern std::vector<std::vector<std::string_view>> fc8(std::string_view subject, std::size_t limit);

static constexpr const char r9_key[] = R"((?:[0-9]*).*?\((.*)%)";
extern std::vector<std::vector<std::string_view>> fc9(std::string_view subject, std::size_t limit);

static constexpr const char r10_key[] = R"(([0-9]*).*?\((?:.*)%)";
extern std::vector<std::vector<std::string_view>> fc10(std::string_view subject, std::size_t limit);

static constexpr const char r11_key[] = R"((.*?)s, .*\. (.*?)/min)";
extern std::vector<std::vector<std::string_view>> fc11(std::string_view subject, std::size_t limit);

static constexpr const char r12_key[] = R"(.*\((.*)%.*)";
extern std::vector<std::vector<std::string_view>> fc12(std::string_view subject, std::size_t limit);

static constexpr const char r13_key[] = R"(([0-9]+) episode.?)";
extern std::vector<std::vector<std::string_view>> fc13(std::string_view subject, std::size_t limit);

static constexpr const char r14_key[] = R"(([0-9]+):([0-9]+):([0-9]+))";
extern std::vector<std::vector<std::string_view>> fc14(std::string_view subject, std::size_t limit);

static constexpr const char r15_key[] = R"(([0-9]{2}).([0-9]{2}).([0-9]{4}) ([0-9]{2}):([0-9]{2}):([0-9]{2}))";
extern std::vector<std::vector<std::string_view>> fc15(std::string_view subject, std::size_t limit);

static constexpr const char r16_key[] = R"(([0-9]+?)/min)";
extern std::vector<std::vector<std::string_view>> fc16(std::string_view subject, std::size_t limit);

static constexpr const char r17_key[] = R"((.*)%)";
extern std::vector<std::vector<std::string_view>> fc17(std::string_view subject, std::size_t limit);

static constexpr const char r18_key[] = R"(([0-9]+) QRS)";
extern std::vector<std::vector<std::string_view>> fc18(std::string_view subject, std::size_t limit);

static constexpr const char r19_key[] = R"((.*?)s.*)";
extern std::vector<std::vector<std::string_view>> fc19(std::string_view subject, std::size_t limit);

static constexpr const char r20_key[] = R"((.*)% disturbed)";
extern std::vector<std::vector<std::string_view>> fc20(std::string_view subject, std::size_t limit);

static constexpr const char r21_key[] = R"(Signal quality:)";
extern std::vector<std::vector<std::string_view>> fc21(std::string_view subject, std::size_t limit);

static constexpr const char r22_key[] = R"(Events:)";
extern std::vector<std::vector<std::string_view>> fc22(std::string_view subject, std::size_t limit);

static constexpr const char r23_key[] = R"(Duration:)";
extern std::vector<std::vector<std::string_view>> fc23(std::string_view subject, std::size_t limit);

static constexpr const char r24_key[] = R"(End:)";
extern std::vector<std::vector<std::string_view>> fc24(std::string_view subject, std::size_t limit);

static constexpr const char r25_key[] = R"(Start:)";
extern std::vector<std::vector<std::string_view>> fc25(std::string_view subject, std::size_t limit);

static constexpr const char r26_key[] = R"(Middle resting:)";
extern std::vector<std::vector<std::string_view>> fc26(std::string_view subject, std::size_t limit);

static constexpr const char r27_key[] = R"(Middle active:)";
extern std::vector<std::vector<std::string_view>> fc27(std::string_view subject, std::size_t limit);

static constexpr const char r28_key[] = R"(Average:)";
extern std::vector<std::vector<std::string_view>> fc28(std::string_view subject, std::size_t limit);

static constexpr const char r29_key[] = R"(Max:)";
extern std::vector<std::vector<std::string_view>> fc29(std::string_view subject, std::size_t limit);

static constexpr const char r30_key[] = R"(Min:)";
extern std::vector<std::vector<std::string_view>> fc30(std::string_view subject, std::size_t limit);

static constexpr const char r31_key[] = R"((Salvos / VTA:)|(No salvos / VTA found))";
extern std::vector<std::vector<std::string_view>> fc31(std::string_view subject, std::size_t limit);

static constexpr const char r32_key[] = R"((Triplets:)|(No triplets found))";
extern std::vector<std::vector<std::string_view>> fc32(std::string_view subject, std::size_t limit);

static constexpr const char r33_key[] = R"((Couplets:)|(No couplets found))";
extern std::vector<std::vector<std::string_view>> fc33(std::string_view subject, std::size_t limit);

static constexpr const char r34_key[] = R"(VPB per 1000 events:)";
extern std::vector<std::vector<std::string_view>> fc34(std::string_view subject, std::size_t limit);

static constexpr const char r35_key[] = R"(VPB per hour:)";
extern std::vector<std::vector<std::string_view>> fc35(std::string_view subject, std::size_t limit);

static constexpr const char r36_key[] = R"(VPB in % of events:)";
extern std::vector<std::vector<std::string_view>> fc36(std::string_view subject, std::size_t limit);

static constexpr const char r37_key[] = R"(Number of VPB:)";
extern std::vector<std::vector<std::string_view>> fc37(std::string_view subject, std::size_t limit);

static constexpr const char r38_key[] = R"(Number of SVPB:)";
extern std::vector<std::vector<std::string_view>> fc38(std::string_view subject, std::size_t limit);

static constexpr const char r39_key[] =
    R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\(([^\n]*)\)Tj\nET)";
extern std::vector<std::vector<std::string_view>> fc39(std::string_view subject, std::size_t limit);

static constexpr const char r40_key[] =
    R"(\((Page [0-9]*)\)Tj\nET)";
extern std::vector<std::vector<std::string_view>> fc40(std::string_view subject, std::size_t limit);

static constexpr const char r41_key[] = R"(BBox [^\n]*?\nstream\n((?:.|\n)*?)endstream)";
extern std::vector<std::vector<std::string_view>> fc41(std::string_view subject, std::size_t limit);

static constexpr const char r42_key[] = R"(\((Page 1)\)Tj)";
extern std::vector<std::vector<std::string_view>> fc42(std::string_view subject, std::size_t limit);


static constexpr const char r43_key[] = R"(\((Page [0-9]*)\)Tj)";
extern std::vector<std::vector<std::string_view>> fc43(std::string_view subject, std::size_t limit);


static constexpr const char r44_key[] = R"(\((Example No.: [0-9]*)\)Tj)";
extern std::vector<std::vector<std::string_view>> fc44(std::string_view subject, std::size_t limit);


static constexpr const char r45_key[] =
    R"(BT\n/F(\d+) ([\d.]+) Tf\n([^\n]*) ([\d.]*) ([\d.]*) Tm\n\((Example No.: [\d]*?|Page [\d]*?)\)Tj\nET)";
extern std::vector<std::vector<std::string_view>> fc45(std::string_view subject, std::size_t limit);


static constexpr const char r46_key[] = R"(\((Page 2)\)Tj)";
extern std::vector<std::vector<std::string_view>> fc46(std::string_view subject, std::size_t limit);


static constexpr const char r47_key[] = R"(\((Numerical overview of the arrhythmia)\)Tj)";
extern std::vector<std::vector<std::string_view>> fc47(std::string_view subject, std::size_t limit);


static constexpr const char r48_key[] = R"(\((Frequency analysis and ST analysis)\)Tj)";
extern std::vector<std::vector<std::string_view>> fc48(std::string_view subject, std::size_t limit);


static constexpr const char r49_key[] = R"((Bradycardias:)|(No bradycardia found))";
extern std::vector<std::vector<std::string_view>> fc49(std::string_view subject, std::size_t limit);

static constexpr const char r50_key[] = R"(Report)";
extern std::vector<std::vector<std::string_view>> fc50(std::string_view subject, std::size_t limit);

static constexpr const char r51_key[] = R"(SVPB)";
extern std::vector<std::vector<std::string_view>> fc51(std::string_view subject, std::size_t limit);

static constexpr const char r52_key[] = R"(Total:)";
extern std::vector<std::vector<std::string_view>> fc52(std::string_view subject, std::size_t limit);

static constexpr const char r53_key[] = R"(Time)";
extern std::vector<std::vector<std::string_view>> fc53(std::string_view subject, std::size_t limit);

static constexpr const char r54_key[] = R"(HR average)";
extern std::vector<std::vector<std::string_view>> fc54(std::string_view subject, std::size_t limit);

static constexpr const char r55_key[] = R"(Result:)";
extern std::vector<std::vector<std::string_view>> fc55(std::string_view subject, std::size_t limit);

static constexpr const char r56_key[] = R"((.*))";
extern std::vector<std::vector<std::string_view>> fc56(std::string_view subject, std::size_t limit);

static constexpr const char r57_key[] = R"((.*)/min.*)";
extern std::vector<std::vector<std::string_view>> fc57(std::string_view subject, std::size_t limit);

static constexpr const char r58_key[] = R"(Page [0-9]*)";
extern std::vector<std::vector<std::string_view>> fc58(std::string_view subject, std::size_t limit);

static constexpr const char r59_key[] = R"(Patient ID)";
extern std::vector<std::vector<std::string_view>> fc59(std::string_view subject, std::size_t limit);

static constexpr const char r60_key[] = R"(First name)";
extern std::vector<std::vector<std::string_view>> fc60(std::string_view subject, std::size_t limit);

static constexpr const char r61_key[] = R"(Date of birth)";
extern std::vector<std::vector<std::string_view>> fc61(std::string_view subject, std::size_t limit);

static constexpr const char r62_key[] = R"(Sex)";
extern std::vector<std::vector<std::string_view>> fc62(std::string_view subject, std::size_t limit);

static constexpr const char r63_key[] = R"(Surname)";
extern std::vector<std::vector<std::string_view>> fc63(std::string_view subject, std::size_t limit);

static constexpr const char r64_key[] = R"(Length (\d+))";
extern std::vector<std::vector<std::string_view>> fc64(std::string_view subject, std::size_t limit);

static constexpr const char r65_key[] = R"(\d+ \d+ obj\n)";
extern std::vector<std::vector<std::string_view>> fc65(std::string_view subject, std::size_t limit);

static constexpr const char r66_key[] = R"(/XObject /Length (\d+))";
extern std::vector<std::vector<std::string_view>> fc66(std::string_view subject, std::size_t limit);

template <const auto&... FSes>
struct FxStrList {};

template <auto*... Fcns>
struct FcnPtrList {};

using RegexList = FcnPtrList<&fc0,&fc1,&fc2,&fc3,&fc4,&fc5,&fc6,&fc7,&fc8,&fc9,&fc10,&fc11,&fc12,&fc13,&fc14,&fc15,&fc16,&fc17,&fc18,&fc19,&fc20,&fc21,&fc22,&fc23,&fc24,&fc25,&fc26,&fc27,&fc28,&fc29,&fc30,&fc31,&fc32,&fc33,&fc34,&fc35,&fc36,&fc37,&fc38,&fc39,&fc40,&fc41,&fc42,&fc43,&fc44,&fc45,&fc46,&fc47,&fc48,&fc49,&fc50,&fc51,&fc52,&fc53,&fc54,&fc55,&fc56,&fc57,fc58,fc59,fc60,fc61,fc62,fc63, fc64, fc65, fc66>;
using SvList = FxStrList<r0_key,r1_key,r2_key,r3_key,r4_key,r5_key,r6_key,r7_key,r8_key,r9_key,r10_key,r11_key,r12_key,r13_key,r14_key,r15_key,r16_key,r17_key,r18_key,r19_key,r20_key,r21_key,r22_key,r23_key,r24_key,r25_key,r26_key,r27_key,r28_key,r29_key,r30_key,r31_key,r32_key,r33_key,r34_key,r35_key,r36_key,r37_key,r38_key,r39_key,r40_key,r41_key,r42_key,r43_key,r44_key,r45_key,r46_key,r47_key,r48_key, r49_key,r50_key,r51_key,r52_key,r53_key,r54_key,r55_key,r56_key,r57_key,r58_key,r59_key,r60_key,r61_key,r62_key,r63_key, r64_key,r65_key, r66_key>;

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
