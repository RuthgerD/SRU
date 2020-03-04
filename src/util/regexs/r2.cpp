#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r2 = ctll::fixed_string{r2_key};
    auto fc2(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r2>(subject, limit);
    }
}