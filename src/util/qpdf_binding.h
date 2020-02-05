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
    Qpdf(const std::filesystem::path& cache_path);

    void getVersion();
    std::filesystem::path decompress(const std::filesystem::path& pdf_file) const;
    const std::filesystem::path compress(std::filesystem::path pdf_file) const;
    const std::filesystem::path& getCachePath() const;
};
}; // namespace sru::util