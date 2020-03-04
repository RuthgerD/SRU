#include <ctre.hpp>
#include "re_accel.h"
namespace sru::re {
template <class Match, std::size_t... Is> auto match2vec_impl(Match match, std::index_sequence<Is...>) {
    std::vector<std::string_view> ret;
    (ret.push_back(match.template get<Is>().to_view()), ...);
    return ret;
}

template <class Match> auto match2vec(Match match) -> std::vector<std::string_view> {
    return match2vec_impl(match, std::make_index_sequence<std::tuple_size_v<Match>>{});
}
template <const auto& RE> auto fc(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
    std::vector<std::vector<std::string_view>> ret;
    auto range = ctre::range<RE>(subject);
    ret.reserve(limit != -1 ? limit : 64);
    auto it = range.begin();
    while (it != range.end() && limit-- != 0)
        ret.push_back(match2vec(*it++));
    return ret;
}
}
