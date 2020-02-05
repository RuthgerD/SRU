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
    std::unordered_map<int, std::vector<std::reference_wrapper<sru::pdf::StringObject>>> marked_objs;
    std::unordered_map<int, std::vector<std::reference_wrapper<sru::pdf::StringObject>>> stickied_objs;
    std::unordered_map<int, std::reference_wrapper<sru::pdf::StringObject>> anchor_objs;

    const PageConfig config;

  public:
    PdfPage(const PdfPage& oth) = delete;
    PdfPage(PdfPage&&) = default;
    PdfPage& operator=(const PdfPage& oth) = delete;
    PdfPage& operator=(PdfPage&&) = default;
    PdfPage(std::string raw, const PageConfig config);
    const std::vector<sru::pdf::StringObject>& getObjects() const;
    std::vector<std::reference_wrapper<sru::pdf::StringObject>> getMarkedObjects(int id) const;
    void indexObjects();
    void printObjects() const;
};

}; // namespace sru::pdf
