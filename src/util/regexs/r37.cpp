#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r37 = ctll::fixed_string{r37_key};
    auto fc37(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r37>(subject, limit);
    }
}