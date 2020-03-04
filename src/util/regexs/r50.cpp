#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r50 = ctll::fixed_string{r50_key};
auto fc50(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r50>(subject, limit); }
} // namespace sru::re