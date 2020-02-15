#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace sru::pdf {

struct PageConfig {
    std::string page_name;
    int priority;
    bool copy_to_final;
    std::string regex_id;
    std::string obj_regex;
    std::vector<int> groups;
};
extern std::vector<PageConfig> PageConfigPool;
} // namespace sru::pdf