#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r48 = ctll::fixed_string{r48_key};
auto fc48(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r48>(subject, limit); }
} // namespace sru::re