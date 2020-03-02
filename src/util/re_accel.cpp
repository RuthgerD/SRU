#include "re_accel.h"
#include <ctre.hpp>

namespace sru::re {

template <class Match, std::size_t... Is> auto match2vec_impl(Match match, std::index_sequence<Is...>) {
    std::vector<std::string_view> ret;
    (ret.push_back(match.template get<Is>().to_view()), ...);
    return ret;
}

template <class Match> auto match2vec(Match match) -> std::vector<std::string_view> {
    return match2vec_impl(match, std::make_index_sequence<std::tuple_size_v<Match>>{});
}

using LType = std::vector<std::vector<std::string_view>>(*)(std::string_view, std::size_t);

template <const auto&... SVes, const auto&... FSes>
constexpr auto gen_tbl(FxStrList<SVes...>, FxStrList<FSes...>) noexcept
-> std::array<std::pair<std::string_view, LType>, sizeof...(SVes)> {
    static_assert(sizeof...(SVes) == sizeof...(FSes), "There must be as many keys as there are values");
    return {std::pair{SVes, +[](std::string_view subject, std::size_t limit){
      std::vector<std::vector<std::string_view>> ret;
      auto range = ctre::range<FSes>(subject);
      ret.reserve(limit != -1 ? limit : 64);
      auto it = range.begin();
      while(it != range.end() && limit-- != 0)
          ret.push_back(match2vec(*it++));
      return ret;
    }}...};
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