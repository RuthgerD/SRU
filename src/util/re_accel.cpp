#include "re_accel.h"
#include <ctre.hpp>

namespace sru::re {




using LType = std::vector<std::vector<std::string_view>>(*)(std::string_view, std::size_t);

template <const auto&... SVes, auto*... FSes>
constexpr auto gen_tbl(FxStrList<SVes...>, FcnPtrList<FSes...>) noexcept -> std::array<std::pair<std::string_view, LType>, sizeof...(SVes)> {
    static_assert(sizeof...(SVes) == sizeof...(FSes), "There must be as many keys as there are values");
    return {std::pair{SVes, FSes}...};
}

constexpr static auto re_table = gen_tbl(SvList{}, RegexList{});

auto ReAccel::getAllMatches(std::string_view key, std::string_view sv, int limit) -> OptMatchResults {
    if (auto ctre_re = std::find_if(re_table.begin(), re_table.end(), [&](const auto& ctre) { return key == ctre.first; });
        ctre_re != re_table.end()) {
        return ctre_re->second(sv, limit);
    }
    return {};
}
} // namespace sru::re