#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace sru::pdf {

struct ObjectConfig {
  // Styling config
  std::string text_justify;
  int maximum_values;
  float y_object_spacing;
  float round_cut_off;
  bool is_int;
  float minimum_value;

  // Calculation config
  std::vector<std::string> calc_modes;
  //// For AVRG mode
  bool avrg_self;
  std::string avrg_source_group;
  std::string avrg_source_sub_group;
  std::string avrg_base_group;
  std::string avrg_base_sub_group;
  int avrg_multiplier;
  //// For ADDITION mode
  float overflow_threshold;
  //// For SORTING mode
  std::vector<int> sort_settings;
  std::string re_comp_name;
  int re_group_count;
  std::vector<std::string> regexs;

  // Indexing config
  float margin_x;
  float margin_y;
  int object_count;

  /*

          "text_justify": -2.3,
          "roundCutOff": 1,
          "modes": ["add", "avrg"],
          "avrg_source_group": "Events",
          "avrg_source_sub_group": "amount",
          "avrg_base_group": "Number of SVPB",
          "avrg_base_sub_group": "amount/avrg"
          "avrg_self": true,
          "avrg_multiplier": 1,
          "cutoff_threshold": 1,
          "is_int": false,
          "overflow": 60,
          "sort_settings": [-1, 1],
          "sort_limit": 3,
          "YObjectSpacing": 11.33,
          "regex_group": "comp",
          "regexs": ["(?P<comp1>.*?)s, .*\\. (?P<comp0>.*?)/min"],
          "regexgroups": 2
          "margin_x": 20,
          "margin_y": -40,
          "objects": 3


  */
};
}; // namespace sru::pdf