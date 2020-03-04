#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r3 = ctll::fixed_string{r3_key};
    auto fc3(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r3>(subject, limit);
    }
}