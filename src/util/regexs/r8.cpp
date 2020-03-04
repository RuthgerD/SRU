#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r8 = ctll::fixed_string{r8_key};
    auto fc8(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r8>(subject, limit);
    }
}