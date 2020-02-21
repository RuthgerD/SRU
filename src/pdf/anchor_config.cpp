#include "anchor_config.h"
#include <optional>
#include <vector>

namespace sru::pdf {
std::vector<AnchorConfig> AnchorConfigPool;

auto getAnchorConfig(size_t id) -> std::optional<AnchorConfig> {
    for (const auto& conf : sru::pdf::AnchorConfigPool) {
        if (conf.id == id) {
            return conf;
        }
    }
    return {};
}
} // namespace sru::pdf