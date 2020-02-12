#pragma once
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace sru::pdf {
struct ObjectConfig {
    int id;
    std::string name;

    float text_justify;
    int maximum_values;
    float y_object_spacing;
    float round_cut_off;
    int decimal_points;

    std::vector<std::string> calc_modes;

    bool avrg_self;
    int avrg_source_id;
    int avrg_base_id;
    int avrg_multiplier;

    float overflow_threshold;

    std::vector<bool> sort_settings;
    std::vector<int> re_extract_order;
    std::vector<std::string> regexs;

    float margin_x;
    float margin_y;
    int object_count;
    int sticky_id;
};
extern std::vector<ObjectConfig> ObjectConfigPool;
std::optional<ObjectConfig> getObjectConfig(int id);
}; // namespace sru::pdf