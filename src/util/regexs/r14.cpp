#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r14 = ctll::fixed_string{r14_key};
    auto fc14(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r14>(subject, limit);
    }
}