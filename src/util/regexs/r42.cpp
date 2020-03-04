#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r42 = ctll::fixed_string{r42_key};
auto fc42(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r42>(subject, limit); }
} // namespace sru::re