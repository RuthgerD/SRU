#pragma once
#include "../util/util.h"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace sru::pdf {
struct AnchorConfig {
    int id;
    bool is_virtual;
    sru::util::Coordinate position;
    std::string name;
    std::string content_id;
    std::string content_;
    std::string content_alt;
    bool save_anchor;
    std::vector<int> sub_groups;
};
auto getAnchorConfig(int id) -> std::optional<AnchorConfig>;
extern std::vector<AnchorConfig> AnchorConfigPool;

} // namespace sru::pdf