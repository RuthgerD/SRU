#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r16 = ctll::fixed_string{r16_key};
    auto fc16(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r16>(subject, limit);
    }
}