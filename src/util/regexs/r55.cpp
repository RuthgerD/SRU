#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r55 = ctll::fixed_string{r55_key};
auto fc55(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r55>(subject, limit); }
} // namespace sru::re