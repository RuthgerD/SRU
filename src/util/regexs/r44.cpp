#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r44 = ctll::fixed_string{r44_key};
auto fc44(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r44>(subject, limit); }
} // namespace sru::re