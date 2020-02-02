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
    bool is_int;
    float minimum_value;

    std::vector<std::string> calc_modes;

    bool avrg_self;
    std::string avrg_source_group;
    std::string avrg_source_sub_group;
    std::string avrg_base_group;
    std::string avrg_base_sub_group;
    int avrg_multiplier;

    float overflow_threshold;

    std::vector<bool> sort_settings;
    std::string re_comp_name;
    int re_group_count;
    std::vector<std::string> regexs;

    float margin_x;
    float margin_y;
    int object_count;
};
extern std::vector<ObjectConfig> ObjectConfigPool;
const std::optional<ObjectConfig> getObjectConfig(int id);
}; // namespace sru::pdf