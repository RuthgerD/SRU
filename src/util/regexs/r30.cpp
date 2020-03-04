#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r30 = ctll::fixed_string{r30_key};
    auto fc30(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r30>(subject, limit);
    }
}