#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r28 = ctll::fixed_string{r28_key};
    auto fc28(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r28>(subject, limit);
    }
}