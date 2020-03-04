#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r53 = ctll::fixed_string{r53_key};
auto fc53(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r53>(subject, limit); }
} // namespace sru::re