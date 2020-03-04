#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r11 = ctll::fixed_string{r11_key};
    auto fc11(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r11>(subject, limit);
    }
}