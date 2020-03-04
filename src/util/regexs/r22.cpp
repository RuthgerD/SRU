#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r22 = ctll::fixed_string{r22_key};
    auto fc22(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r22>(subject, limit);
    }
}