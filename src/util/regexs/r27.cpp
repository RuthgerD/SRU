#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r27 = ctll::fixed_string{r27_key};
    auto fc27(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r27>(subject, limit);
    }
}