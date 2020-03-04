#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r62 = ctll::fixed_string{r62_key};
auto fc62(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r62>(subject, limit); }
} // namespace sru::re