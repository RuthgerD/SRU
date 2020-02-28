#pragma once
#include "../util/util.h"
#include "page_config.h"
#include "string_obj.h"
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <rapidjson/document.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace sru::pdf {
using Offset = std::vector<sru::pdf::StringObject>::size_type;
class PdfPage {
    std::string raw_;
    std::vector<sru::pdf::StringObject> objs_;
    std::unordered_map<int, std::vector<Offset>> marked_objs_;
    std::unordered_map<int, Offset> anchor_objs_;
    std::unordered_map<int, sru::util::Coordinate> anchor_positions_;

    std::unordered_map<Offset, StringObject> update_staging_;
    std::vector<Offset> delete_staging_;
    std::vector<StringObject> insert_staging_;
    PageConfig config_;

  public:
    PdfPage(const PdfPage& oth) = default;
    PdfPage(PdfPage&&) = default;
    PdfPage& operator=(const PdfPage& oth) = default;
    PdfPage& operator=(PdfPage&&) = default;
    PdfPage(std::string raw, const PageConfig& config);
    auto getObjects() -> const std::vector<StringObject>&;
    auto getMarkedObjects(size_t id) -> std::vector<Offset>;
    auto updateObject(Offset id, StringObject& obj) -> bool;
    auto deleteObject(Offset id) -> bool;
    auto insertObject(StringObject& obj) -> void;
    auto commit() -> bool;
    auto clear_staging() -> void;
    auto getObject(Offset id) const -> const StringObject&;
    auto getConfig() const -> PageConfig;
    auto getAnchorPositions() const -> const std::unordered_map<int, sru::util::Coordinate>&;
    auto getAnchorObjects() const -> const std::unordered_map<int, Offset>&;
    auto getRaw() const -> const std::string&;
    void indexObjects();
    void printObjects() const;
};

} // namespace sru::pdf
