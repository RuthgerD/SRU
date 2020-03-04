#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r36 = ctll::fixed_string{r36_key};
    auto fc36(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r36>(subject, limit);
    }
}