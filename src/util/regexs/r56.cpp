#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r56 = ctll::fixed_string{r56_key};
auto fc56(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r56>(subject, limit); }
} // namespace sru::re