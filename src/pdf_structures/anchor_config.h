#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace sru::pdf {
struct AnchorConfig {
    int id;
    std::string content_id;
    std::string content_;
    std::string content_alt;
    bool save_anchor;
    std::vector<int> sub_groups;
};
static std::vector<AnchorConfig> AnchorConfigPool;

}; // namespace sru::pdf