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
class PdfPage {
    std::string raw;
    std::vector<sru::pdf::StringObject> objs;
    std::unordered_map<int, std::vector<int>> marked_objs;
    std::unordered_map<int, std::vector<int>> stickied_objs;
    std::unordered_map<int, int> anchor_objs;

    PageConfig config;

  public:
    PdfPage(const PdfPage& oth) = default;
    PdfPage(PdfPage&&) = default;
    PdfPage& operator=(const PdfPage& oth) = default;
    PdfPage& operator=(PdfPage&&) = default;
    PdfPage(std::string raw, const PageConfig& config);
    const std::vector<sru::pdf::StringObject>& getObjects() const;
    auto getMarkedObjects(int id) -> std::vector<std::reference_wrapper<StringObject>>;

    auto getRaw() const -> const std::string&;
    void indexObjects();
    void printObjects() const;
};

} // namespace sru::pdf
