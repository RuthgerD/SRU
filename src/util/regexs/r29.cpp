#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r29 = ctll::fixed_string{r29_key};
    auto fc29(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r29>(subject, limit);
    }
}