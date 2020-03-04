#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r23 = ctll::fixed_string{r23_key};
    auto fc23(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r23>(subject, limit);
    }
}