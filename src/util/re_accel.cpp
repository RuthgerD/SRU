#include "re_accel.h"
#include <ctre.hpp>

namespace sru::util {

static constexpr std::tuple ctre_regexs = {
    std::pair{default_obj_match_key, [](std::string_view sv) { return ctre::range<default_obj_match>(sv); }},
    std::pair{PAGE_EXAMPLE_obj_match_key, [](std::string_view sv) { return ctre::range<PAGE_EXAMPLE_obj_match>(sv); }},
    std::pair{page_match_key, [](std::string_view sv) { return ctre::range<page_match>(sv); }},
    std::pair{page_1_id_key, [](std::string_view sv) { return ctre::range<page_1_id>(sv); }},
    std::pair{page_any_id_key, [](std::string_view sv) { return ctre::range<page_any_id>(sv); }},
    std::pair{page_example_id_key, [](std::string_view sv) { return ctre::range<page_example_id>(sv); }},
    std::pair{page_example_obj_key, [](std::string_view sv) { return ctre::range<page_example_obj_id>(sv); }}};

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
} // namespace sru::util