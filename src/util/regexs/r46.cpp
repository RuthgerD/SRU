#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r46 = ctll::fixed_string{r46_key};
auto fc46(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r46>(subject, limit); }
} // namespace sru::re