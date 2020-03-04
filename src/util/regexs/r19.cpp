#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r19 = ctll::fixed_string{r19_key};
    auto fc19(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r19>(subject, limit);
    }
}