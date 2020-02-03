#include "pdf_cluster.h"

namespace sru::pdf {
PdfCluster::PdfCluster(std::vector<std::filesystem::path> pdf_file_paths, sru::util::Qpdf qpdf) : qpdf{qpdf} {
    for (auto& path : pdf_file_paths) {
        auto deflated = qpdf.decompress(path);
        pdf_files.emplace_back(deflated);
        std::cout << deflated.generic_string() << "\n";
    }
}
} // namespace sru::pdf