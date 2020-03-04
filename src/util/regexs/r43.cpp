#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r43 = ctll::fixed_string{r43_key};
auto fc43(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r43>(subject, limit); }
} // namespace sru::re