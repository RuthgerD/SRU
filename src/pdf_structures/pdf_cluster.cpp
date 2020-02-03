#include "pdf_cluster.h"

namespace sru::pdf {
PdfCluster::PdfCluster(std::vector<std::filesystem::path> pdf_file_paths, sru::util::Qpdf qpdf) : qpdf{qpdf} {
    std::vector<std::future<PdfFile>> result;
    for (const auto& path : pdf_file_paths) {
        result.push_back(std::async([&]() {
            const auto deflated = qpdf.decompress(path);
            std::cout << deflated.generic_string() << "\n";
            return PdfFile{deflated};
        }));
    }
    for (auto& x : result) {
        pdf_files.push_back(x.get());
    }
}
} // namespace sru::pdf