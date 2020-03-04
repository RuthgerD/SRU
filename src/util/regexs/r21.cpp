#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r21 = ctll::fixed_string{r21_key};
    auto fc21(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r21>(subject, limit);
    }
}