#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r6 = ctll::fixed_string{r6_key};
    auto fc6(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r6>(subject, limit);
    }
}