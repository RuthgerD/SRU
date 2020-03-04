#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r54 = ctll::fixed_string{r54_key};
auto fc54(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r54>(subject, limit); }
} // namespace sru::re