#include "calc_config.h"

#include <optional>
#include <vector>

namespace sru::pdf {
std::vector<CalcConfig> CalcConfigPool;
auto getCalcConfig(size_t id) -> std::optional<CalcConfig> {
    for (const auto& conf : CalcConfigPool) {
        if (conf.id == id) {
            return conf;
        }
    }
    return {};
}
} // namespace sru::pdf