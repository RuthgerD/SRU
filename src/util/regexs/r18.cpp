#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r18 = ctll::fixed_string{r18_key};
    auto fc18(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r18>(subject, limit);
    }
}