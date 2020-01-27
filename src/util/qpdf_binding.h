#pragma once
#include <filesystem>
#include <string>

namespace sru::util {
class Qpdf {
    std::filesystem::path cache_path;
// stupid hard coding :)
#ifdef __linux__
    const std::string bin{"qpdf"};
#else
    const std::string bin{"qpdf/qpdf.exe"};
#endif
  public:
    Qpdf(std::filesystem::path cache_path)
        : cache_path{cache_path.lexically_normal()} {}

    void getVersion();
    const std::filesystem::path
    decompress(std::filesystem::path pdf_file) const;
    const std::filesystem::path compress(std::filesystem::path pdf_file) const;
    const std::filesystem::path &getCachePath() const;
};
}; // namespace sru::util