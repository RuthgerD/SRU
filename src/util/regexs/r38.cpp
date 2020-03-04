#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r38 = ctll::fixed_string{r38_key};
    auto fc38(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r38>(subject, limit);
    }
}