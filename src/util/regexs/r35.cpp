#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r35 = ctll::fixed_string{r35_key};
    auto fc35(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r35>(subject, limit);
    }
}