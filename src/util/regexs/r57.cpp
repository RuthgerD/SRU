#include "../fc.h"
#include "../re_accel.h"
namespace sru::re {
static constexpr auto r57 = ctll::fixed_string{r57_key};
auto fc57(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> { return fc<r57>(subject, limit); }
} // namespace sru::re