#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r58 = ctll::fixed_string{r58_key};
auto fc58(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r58>(subject, limit); }
} // namespace sru::re