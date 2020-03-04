#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r61 = ctll::fixed_string{r61_key};
auto fc61(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r61>(subject, limit); }
} // namespace sru::re