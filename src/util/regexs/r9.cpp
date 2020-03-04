#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r9 = ctll::fixed_string{r9_key};
    auto fc9(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r9>(subject, limit);
    }
}