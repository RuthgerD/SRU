#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r45 = ctll::fixed_string{r45_key};
auto fc45(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r45>(subject, limit); }
} // namespace sru::re