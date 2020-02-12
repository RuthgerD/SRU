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
static float timepool = 0;
class PdfFile {
    std::string raw;
    std::vector<PdfPage> pages;

    std::filesystem::path path;

  public:
    explicit PdfFile(const std::string& raw, std::filesystem::path path);
    [[nodiscard]] auto getPages() const -> const std::vector<PdfPage>&;
    [[nodiscard]] auto getRaw() const -> const std::string&;
    [[nodiscard]] auto getMarkedObjects(int id) const -> std::vector<std::reference_wrapper<sru::pdf::StringObject>>;
    auto deletePage(unsigned int page_no) -> bool;
    auto deletePage(sru::pdf::PdfPage& page) -> bool;

};
}; // namespace sru::pdf