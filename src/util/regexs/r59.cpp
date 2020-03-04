#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r59 = ctll::fixed_string{r59_key};
auto fc59(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r59>(subject, limit); }
} // namespace sru::re