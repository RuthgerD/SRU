#include "../re_accel.h"
#include "../fc.h"
namespace sru::re {
	static constexpr auto r13 = ctll::fixed_string{r13_key};
    auto fc13(std::string_view subject, std::size_t limit) -> std::vector<std::vector<std::string_view>> {
        return fc<r13>(subject, limit);
    }
}