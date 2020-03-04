#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r15 = ctll::fixed_string{r15_key};
    auto fc15(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r15>(subject, limit);
    }
}