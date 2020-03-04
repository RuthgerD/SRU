#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r47 = ctll::fixed_string{r47_key};
auto fc47(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r47>(subject, limit); }
} // namespace sru::re