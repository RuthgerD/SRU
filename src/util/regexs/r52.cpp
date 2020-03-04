#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r52 = ctll::fixed_string{r52_key};
auto fc52(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r52>(subject, limit); }
} // namespace sru::re