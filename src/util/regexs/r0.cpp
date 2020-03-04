#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r0 = ctll::fixed_string{r0_key};
    auto fc0(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r0>(subject, limit);
    }
}