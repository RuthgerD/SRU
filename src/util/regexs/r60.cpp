#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r60 = ctll::fixed_string{r60_key};
auto fc60(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r60>(subject, limit); }
} // namespace sru::re