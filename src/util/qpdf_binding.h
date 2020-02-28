#pragma once
#include "../pdf/pdf_file.h"
#include "util.h"
#include <filesystem>
#include <optional>
#include <string>

namespace sru::qpdf {
auto set_cache_path(std::filesystem::path& path) -> void;
auto decompress(const std::filesystem::path& pdf_file) -> std::optional<std::filesystem::path>;
auto compress(const std::filesystem::path& pdf_file) -> std::optional<std::filesystem::path>;
auto delete_page(const sru::pdf::PdfFile& pdf_file, size_t page_no) -> bool;
auto insert_page(const sru::pdf::PdfFile& pdf_file_out, size_t page_no, const sru::pdf::PdfFile& pdf_file_in, size_t new_page_no) -> bool;
auto append_page(const sru::pdf::PdfFile& pdf_file_out, size_t page_no, const sru::pdf::PdfFile& pdf_file_in, size_t new_page_no) -> bool;
auto increase_size(const std::filesystem::path& path, size_t from, size_t size) -> bool;
auto decrease_size(const std::filesystem::path& path, size_t from, size_t size) -> bool;
auto empty_cache() -> void;

} // namespace sru::qpdf
