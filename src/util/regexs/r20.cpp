#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r20 = ctll::fixed_string{r20_key};
    auto fc20(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r20>(subject, limit);
    }
}