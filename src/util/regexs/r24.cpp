#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r24 = ctll::fixed_string{r24_key};
    auto fc24(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r24>(subject, limit);
    }
}