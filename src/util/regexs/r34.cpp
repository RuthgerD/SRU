#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r34 = ctll::fixed_string{r34_key};
    auto fc34(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r34>(subject, limit);
    }
}