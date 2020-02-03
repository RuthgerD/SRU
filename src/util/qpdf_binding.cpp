#include "qpdf_binding.h"
#include "util.h"
#include <cstdlib>
#include <filesystem>
#include <string>

namespace sru::util {
Qpdf::Qpdf(std::filesystem::path cache_path) : cache_path{cache_path.lexically_normal()} {}
void Qpdf::getVersion() { cmd(bin + " --version"); };

const std::filesystem::path& Qpdf::getCachePath() const {
    if (!std::filesystem::is_directory(cache_path)) {
        std::filesystem::create_directories(cache_path);
    }
    return cache_path;
}

const std::filesystem::path Qpdf::decompress(std::filesystem::path pdf_file) const {
    const auto abs_pdf_file = std::filesystem::absolute(pdf_file.lexically_normal());
    auto abs_cache_path = std::filesystem::absolute(getCachePath());
    abs_cache_path.append(pdf_file.filename().generic_string());

    auto command = bin + " --stream-data=uncompress \"" + abs_pdf_file.generic_string() + "\" -- \"" + abs_cache_path.generic_string() + "\"";

    cmd(command);

    return abs_cache_path;
}

}; // namespace sru::util
