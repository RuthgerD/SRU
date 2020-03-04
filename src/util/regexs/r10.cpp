#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r10 = ctll::fixed_string{r10_key};
    auto fc10(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r10>(subject, limit);
    }
}