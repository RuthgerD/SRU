#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r25 = ctll::fixed_string{r25_key};
    auto fc25(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r25>(subject, limit);
    }
}