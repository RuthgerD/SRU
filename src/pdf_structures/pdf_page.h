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
class PdfPage {
    std::string raw_;
    std::vector<sru::pdf::StringObject> objs_;
    std::unordered_map<int, std::vector<int>> marked_objs_;
    std::unordered_map<int, std::vector<int>> stickied_objs_;
    std::unordered_map<int, int> anchor_objs_;
    std::unordered_map<int, sru::util::Coordinate> anchor_positions_;

    std::unordered_map<int, StringObject> update_staging_;
    std::vector<int> delete_staging_;
    std::vector<StringObject> insert_staging_;
    PageConfig config_;

  public:
    PdfPage(const PdfPage& oth) = default;
    PdfPage(PdfPage&&) = default;
    PdfPage& operator=(const PdfPage& oth) = default;
    PdfPage& operator=(PdfPage&&) = default;
    PdfPage(std::string raw, const PageConfig& config);
    auto getObjects() const -> const std::vector<sru::pdf::StringObject>&;
    auto getObjects() -> std::vector<sru::pdf::StringObject>&;
    auto db_getObjects() -> const std::vector<StringObject>&;
    auto db_getMarkedObjects(int id) -> std::vector<int>;
    auto db_updateObject(int id, StringObject obj) -> bool;
    auto db_deleteObject(int id) -> bool;
    auto db_insertObject(const StringObject& obj) -> void;
    auto db_commit() -> bool;
    auto db_clear_staging() -> void;
    auto getConfig() const -> PageConfig;
    auto getMarkedObjects(int id) -> std::vector<std::reference_wrapper<StringObject>>;
    auto getAnchorPositions() const -> const std::unordered_map<int, sru::util::Coordinate>&;
    auto getRaw() const -> const std::string&;
    void indexObjects();
    void printObjects() const;

};

} // namespace sru::pdf
