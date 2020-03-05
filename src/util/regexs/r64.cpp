#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r64 = ctll::fixed_string{r64_key};
auto fc64(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r64>(subject, limit); }
} // namespace sru::re