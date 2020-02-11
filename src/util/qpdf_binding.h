#pragma once
#include <filesystem>
#include <optional>
#include <string>

namespace sru::qpdf {
auto set_cache_path(std::filesystem::path& path) -> void;
auto decompress(const std::filesystem::path& pdf_file) -> std::optional<std::filesystem::path>;
auto compress(const std::filesystem::path& pdf_file) -> std::optional<std::filesystem::path>;

} // namespace sru::qpdf