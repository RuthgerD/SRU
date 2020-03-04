#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r12 = ctll::fixed_string{r12_key};
    auto fc12(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r12>(subject, limit);
    }
}