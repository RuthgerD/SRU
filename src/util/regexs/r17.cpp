#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r17 = ctll::fixed_string{r17_key};
    auto fc17(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r17>(subject, limit);
    }
}