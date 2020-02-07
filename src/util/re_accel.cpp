#include "re_accel.h"

namespace sru::util {

    static constexpr std::tuple ctre_regexs = {
        std::pair{default_obj_match_key, [](std::string_view sv) { return ctre::range<default_obj_match>(sv); }},
        std::pair{PAGE_EXAMPLE_obj_match_key, [](std::string_view sv) { return ctre::range<PAGE_EXAMPLE_obj_match>(sv); }},
        std::pair{page_match_key, [](std::string_view sv) { return ctre::range<page_match>(sv); }},
        std::pair{page_1_id_key, [](std::string_view sv) { return ctre::range<page_1_id>(sv); }},
        std::pair{page_any_id_key, [](std::string_view sv) { return ctre::range<page_any_id>(sv); }}
    };

    auto ReAccel::getAllMatches(std::string_view key, std::string_view sv)  -> OptMatchResults {
        bool done = false;
        OptMatchResults ret{};
        visit(ctre_regexs, [&](const auto& e) {
          if (done)
              return;
          if (e.first == key) {
              auto& res = ret.emplace();
              for (const auto& m : e.second(sv))
                  res.push_back(match2vec(m));
              done = true;
          }
        });
        return ret;
    }
}