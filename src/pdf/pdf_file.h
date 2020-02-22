#pragma once
#include "../util/util.h"
#include "anchor_config.h"
#include "pdf_page.h"
#include "string_obj.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace sru::pdf {
class PdfFile {
    std::string raw_;
    size_t total_pages_;
    std::vector<std::pair<size_t, PdfPage>> pages_;

    std::filesystem::path path_;
    size_t real_pages_;

  public:
    explicit PdfFile(const std::string& raw, std::filesystem::path path);
    [[nodiscard]] auto getPages() const -> const std::vector<std::pair<size_t, PdfPage>>&;
    [[nodiscard]] auto getPage(size_t page_no) const -> const PdfPage&;
    [[nodiscard]] auto getPages() -> std::vector<std::pair<size_t, PdfPage>>&;
    [[nodiscard]] auto getPage(size_t page_no) -> PdfPage&;
    [[nodiscard]] auto getRaw() -> std::string;
    [[nodiscard]] auto getMarkedObjects(int id) -> std::vector<std::pair<int, std::vector<offset>>>;
    [[nodiscard]] auto getPath() const -> const std::filesystem::path&;
    [[nodiscard]] auto getPageCount() const -> size_t;
    [[nodiscard]] auto getRealPageCount() const -> size_t;
    auto deletePage(const PdfPage& page) -> bool;
    auto insertPage(PdfPage new_page, size_t new_page_no) -> void;
    auto insertPages(std::vector<PdfPage> new_pages, size_t new_page_no) -> void;
};
} // namespace sru::pdf