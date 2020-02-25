#pragma once
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace sru::pdf {
extern int DATE_PROVIDER;

struct ObjectConfig {
    size_t id;
    std::string name;

    float margin_x;
    float margin_y;
    int anchor_margin_x;
    int anchor_margin_y;
    size_t object_count;
    float y_object_spacing;
    int sticky_id;
    std::vector<size_t> calcs;
};
extern std::vector<ObjectConfig> ObjectConfigPool;
auto getObjectConfig(size_t id) -> std::optional<ObjectConfig>;
} // namespace sru::pdf