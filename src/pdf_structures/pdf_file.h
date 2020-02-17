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
    unsigned int real_pages;

  public:
    explicit PdfFile(const std::string& raw, std::filesystem::path path);
    [[nodiscard]] auto getPages() const -> const std::vector<std::pair<unsigned int, PdfPage>>&;
    [[nodiscard]] auto getPage(unsigned int page_no) const -> const std::pair<unsigned int, PdfPage>&;
    [[nodiscard]] auto getPages() -> std::vector<std::pair<unsigned int, PdfPage>>&;
    [[nodiscard]] auto getPage(unsigned int page_no) -> std::pair<unsigned int, PdfPage>&;
    [[nodiscard]] auto getRaw() -> std::string;
    [[nodiscard]] auto getMarkedObjects(int id) -> std::vector<std::reference_wrapper<sru::pdf::StringObject>>;
    [[nodiscard]] auto getPath() const -> const std::filesystem::path&;
    [[nodiscard]] auto getPageCount() const -> int;
    auto deletePage(unsigned int page_no) -> bool;
    auto deletePage(const PdfPage& page) -> bool;
    auto insertPage(PdfPage new_page, unsigned int new_page_no) -> void;
    auto insertPages(std::vector<std::pair<unsigned int, PdfPage>> new_pages, unsigned int new_page_no) -> void;
    auto insertPages(std::vector<PdfPage> new_pages, unsigned int new_page_no) -> void;
    auto refreshNumbering() -> void;
};
} // namespace sru::pdf