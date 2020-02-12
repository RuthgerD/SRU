#pragma once
#include "../util/re_accel.h"
#include "../util/util.h"
#include "anchor_config.h"
#include "pdf_page.h"
#include "string_obj.h"
#include <boost/regex.hpp>
#include <chrono>
#include <ctll.hpp>
#include <ctre.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace sru::pdf {
class PdfFile {
    std::string raw;
    unsigned int total_pages;
    std::vector<std::pair<unsigned int, PdfPage>> pages;

    std::filesystem::path path;

  public:
    explicit PdfFile(const std::string& raw, std::filesystem::path path);
    [[nodiscard]] auto getPages() const -> const std::vector<std::pair<unsigned int, PdfPage>>&;
    [[nodiscard]] auto getRaw() const -> const std::string&;
    [[nodiscard]] auto getMarkedObjects(int id) const -> std::vector<std::reference_wrapper<sru::pdf::StringObject>>;
    [[nodiscard]] auto getPath() const -> const std::filesystem::path&;
    auto getPageCount() const -> unsigned int;
    auto deletePage(unsigned int page_no) -> bool;
    auto deletePage(const sru::pdf::PdfPage& page) -> bool;

};
}; // namespace sru::pdf