#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r51 = ctll::fixed_string{r51_key};
auto fc51(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r51>(subject, limit); }
} // namespace sru::re