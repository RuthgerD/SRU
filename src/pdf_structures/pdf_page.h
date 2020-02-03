#pragma once
#include "../util/re_accel.h"
#include "../util/util.h"
#include "page_config.h"
#include "string_obj.h"
#include <chrono>
#include <ctll.hpp>
#include <ctre.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <rapidjson/document.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace sru::pdf {
static float pagecounter = 0;
class PdfPage {
    std::string raw;
    std::vector<sru::pdf::StringObject> objs;
    std::unordered_map<int, std::vector<sru::pdf::StringObject*>> marked_objs;
    std::unordered_map<int, std::vector<sru::pdf::StringObject*>> stickied_objs;
    std::unordered_map<int, sru::pdf::StringObject*> anchor_objs;

    const PageConfig config;

  public:
    PdfPage(std::string raw, const PageConfig config);
    const std::vector<sru::pdf::StringObject>& getObjects() const;
    void indexObjects();
    void printObjects();
};

}; // namespace sru::pdf
