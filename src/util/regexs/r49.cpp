#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r49 = ctll::fixed_string{r49_key};
auto fc49(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r49>(subject, limit); }
} // namespace sru::re