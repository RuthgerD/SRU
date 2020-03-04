#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r5 = ctll::fixed_string{r5_key};
    auto fc5(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r5>(subject, limit);
    }
}