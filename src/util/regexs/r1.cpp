#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r1 = ctll::fixed_string{r1_key};
    auto fc1(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r1>(subject, limit);
    }
}