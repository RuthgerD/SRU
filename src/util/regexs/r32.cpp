#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r32 = ctll::fixed_string{r32_key};
    auto fc32(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r32>(subject, limit);
    }
}