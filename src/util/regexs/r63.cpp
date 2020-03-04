#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r63 = ctll::fixed_string{r63_key};
auto fc63(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r63>(subject, limit); }
} // namespace sru::re