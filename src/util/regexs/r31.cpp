#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r31 = ctll::fixed_string{r31_key};
    auto fc31(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r31>(subject, limit);
    }
}