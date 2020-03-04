#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r41 = ctll::fixed_string{r41_key};
auto fc41(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r41>(subject, limit); }
} // namespace sru::re