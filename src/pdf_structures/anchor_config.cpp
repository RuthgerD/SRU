#include "anchor_config.h"
#include <optional>
#include <vector>

namespace sru::pdf {
std::vector<AnchorConfig> AnchorConfigPool;

const std::optional<AnchorConfig> getAnchorConfig(int id) {
    for (const auto &conf : sru::pdf::AnchorConfigPool) {
        if (conf.id == id) {
            return conf;
        }
    }
    return {};
}
}; // namespace sru::pdf