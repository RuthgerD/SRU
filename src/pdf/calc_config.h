#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <array>

namespace sru::pdf {

struct CalcConfig {
    size_t id;
    std::string name;

    float text_justify;
    size_t maximum_values;
    float y_object_spacing;
    float round_cut_off;
    int decimal_points;

    std::string calc_mode;

    bool avrg_self;
    std::array<size_t, 2> avrg_source_id;
    std::array<size_t, 2> avrg_base_id;
    float avrg_multiplier;

    float overflow_threshold;

    std::vector<bool> sort_settings;
    std::vector<int> re_extract_order;
    std::string regex;

};
extern std::vector<CalcConfig> CalcConfigPool;
auto getCalcConfig(size_t id) -> std::optional<CalcConfig>;
} // namespace sru::pdf