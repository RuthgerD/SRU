#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r33 = ctll::fixed_string{r33_key};
    auto fc33(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r33>(subject, limit);
    }
}