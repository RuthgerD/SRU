#include "re_accel.h"
#include <ctre.hpp>

namespace sru::re {

static constexpr std::tuple ctre_regexs = {
    std::pair{default_obj_match_key, [](std::string_view sv) { return ctre::range<default_obj_match>(sv); }},
    std::pair{PAGE_EXAMPLE_obj_match_key, [](std::string_view sv) { return ctre::range<PAGE_EXAMPLE_obj_match>(sv); }},
    std::pair{page_match_key, [](std::string_view sv) { return ctre::range<page_match>(sv); }},
    std::pair{page_1_id_key, [](std::string_view sv) { return ctre::range<page_1_id>(sv); }},
    std::pair{page_any_id_key, [](std::string_view sv) { return ctre::range<page_any_id>(sv); }},
    std::pair{page_example_id_key, [](std::string_view sv) { return ctre::range<page_example_id>(sv); }},
    std::pair{page_example_obj_key, [](std::string_view sv) { return ctre::range<page_example_obj_id>(sv); }},
    std::pair{page_1_plus_key, [](std::string_view sv) { return ctre::range<page_1_plus>(sv); }},
    std::pair{arrhythmia_key, [](std::string_view sv) { return ctre::range<arrhythmia>(sv); }},
    std::pair{r0_key, [](std::string_view sv) { return ctre::range<r0>(sv); }},
    std::pair{r1_key, [](std::string_view sv) { return ctre::range<r1>(sv); }},
    std::pair{r2_key, [](std::string_view sv) { return ctre::range<r2>(sv); }},
    std::pair{r3_key, [](std::string_view sv) { return ctre::range<r3>(sv); }},
    std::pair{r4_key, [](std::string_view sv) { return ctre::range<r4>(sv); }},
    std::pair{r5_key, [](std::string_view sv) { return ctre::range<r5>(sv); }},
    std::pair{r6_key, [](std::string_view sv) { return ctre::range<r6>(sv); }},
    std::pair{r7_key, [](std::string_view sv) { return ctre::range<r7>(sv); }},
    std::pair{r8_key, [](std::string_view sv) { return ctre::range<r8>(sv); }},
    std::pair{r9_key, [](std::string_view sv) { return ctre::range<r9>(sv); }},
    std::pair{r10_key, [](std::string_view sv) { return ctre::range<r10>(sv); }},
    std::pair{r11_key, [](std::string_view sv) { return ctre::range<r11>(sv); }},
    std::pair{r12_key, [](std::string_view sv) { return ctre::range<r12>(sv); }},
    std::pair{r13_key, [](std::string_view sv) { return ctre::range<r13>(sv); }},
    std::pair{r14_key, [](std::string_view sv) { return ctre::range<r14>(sv); }},
    std::pair{r15_key, [](std::string_view sv) { return ctre::range<r15>(sv); }},
    std::pair{r16_key, [](std::string_view sv) { return ctre::range<r16>(sv); }},
    std::pair{r17_key, [](std::string_view sv) { return ctre::range<r17>(sv); }},
    std::pair{r18_key, [](std::string_view sv) { return ctre::range<r18>(sv); }},
    std::pair{r19_key, [](std::string_view sv) { return ctre::range<r19>(sv); }},
    std::pair{r20_key, [](std::string_view sv) { return ctre::range<r20>(sv); }},
    std::pair{r21_key, [](std::string_view sv) { return ctre::range<r21>(sv); }},
    std::pair{r22_key, [](std::string_view sv) { return ctre::range<r22>(sv); }},
    std::pair{r23_key, [](std::string_view sv) { return ctre::range<r23>(sv); }},
    std::pair{r24_key, [](std::string_view sv) { return ctre::range<r24>(sv); }},
    std::pair{r25_key, [](std::string_view sv) { return ctre::range<r25>(sv); }},
    std::pair{r26_key, [](std::string_view sv) { return ctre::range<r26>(sv); }},
    std::pair{r27_key, [](std::string_view sv) { return ctre::range<r27>(sv); }},
    std::pair{r28_key, [](std::string_view sv) { return ctre::range<r28>(sv); }},
    std::pair{r29_key, [](std::string_view sv) { return ctre::range<r29>(sv); }},
    std::pair{r30_key, [](std::string_view sv) { return ctre::range<r30>(sv); }},
    std::pair{r31_key, [](std::string_view sv) { return ctre::range<r31>(sv); }},
    std::pair{r32_key, [](std::string_view sv) { return ctre::range<r32>(sv); }},
    std::pair{r33_key, [](std::string_view sv) { return ctre::range<r33>(sv); }},
    std::pair{r34_key, [](std::string_view sv) { return ctre::range<r34>(sv); }},
    std::pair{r35_key, [](std::string_view sv) { return ctre::range<r35>(sv); }},
    std::pair{r36_key, [](std::string_view sv) { return ctre::range<r36>(sv); }},
    std::pair{r37_key, [](std::string_view sv) { return ctre::range<r37>(sv); }},
    std::pair{r38_key, [](std::string_view sv) { return ctre::range<r38>(sv); }}};

auto ReAccel::getAllMatches(std::string_view key, std::string_view sv, int limit) -> OptMatchResults {
    bool done = false;
    OptMatchResults ret{};
    visit(ctre_regexs, [&](const auto& e) {
        if (done)
            return;
        if (e.first == key) {
            auto& res = ret.emplace();
            int i = 0;
            for (const auto& m : e.second(sv)) {
                if (i > limit && limit > 0) {
                    break;
                }
                res.push_back(match2vec(m));
                ++i;
            }
            done = true;
        }
    });
    return ret;
}
} // namespace sru::re