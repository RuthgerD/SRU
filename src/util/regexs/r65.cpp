#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r65 = ctll::fixed_string{r65_key};
auto fc65(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r65>(subject, limit); }
} // namespace sru::re