#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r66 = ctll::fixed_string{r66_key};
auto fc66(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r66>(subject, limit); }
} // namespace sru::re