#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r7 = ctll::fixed_string{r7_key};
    auto fc7(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r7>(subject, limit);
    }
}