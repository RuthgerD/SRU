#include "object_config.h"
#include <algorithm>
#include <optional>
#include <vector>

namespace sru::pdf {
std::vector<ObjectConfig> ObjectConfigPool;
auto getObjectConfig(size_t id) -> std::optional<ObjectConfig> {
    for (const auto& conf : ObjectConfigPool) {
        if (conf.id == id) {
            return conf;
        }
    }
    return {};
}
} // namespace sru::pdf