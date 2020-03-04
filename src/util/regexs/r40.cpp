#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r40 = ctll::fixed_string{r40_key};
auto fc40(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r40>(subject, limit); }
} // namespace sru::re