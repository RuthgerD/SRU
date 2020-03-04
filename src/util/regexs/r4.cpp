#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r4 = ctll::fixed_string{r4_key};
    auto fc4(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r4>(subject, limit);
    }
}