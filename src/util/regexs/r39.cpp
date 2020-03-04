#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r39 = ctll::fixed_string{r39_key};
    auto fc39(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r39>(subject, limit);
    }
}