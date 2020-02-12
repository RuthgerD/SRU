#include "object_config.h"
#include <algorithm>
#include <optional>
#include <vector>

namespace sru::pdf {
std::vector<ObjectConfig> ObjectConfigPool;
std::optional<ObjectConfig> getObjectConfig(int id) {
    for (const auto conf : ObjectConfigPool) {
        if (conf.id == id) {
            return conf;
        }
    }
    return {};
}
}; // namespace sru::pdf