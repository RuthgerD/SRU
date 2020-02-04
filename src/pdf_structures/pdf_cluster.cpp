#include "pdf_cluster.h"

namespace sru::pdf {
PdfCluster::PdfCluster(std::vector<std::filesystem::path> pdf_file_paths, sru::util::Qpdf qpdf) : qpdf{qpdf} {

    std::vector<std::future<PdfFile>> result;
    for (int i = 0; i < pdf_file_paths.size() - 1; i++) {
        result.push_back(std::async([&]() {
            const auto deflated = qpdf.decompress(pdf_file_paths[i]); // needs checks
            return PdfFile{deflated};
        }));
    }
    const auto deflated = qpdf.decompress(pdf_file_paths.back());

    pdf_files.push_back(PdfFile{deflated});

    for (auto& x : result) {
        auto tmp = x.get();
        pdf_files.push_back(std::move(tmp));
    }

    int index = 0;
    for (auto& x : pdf_files) {
        ++index;
        std::cout << "Pdf " << index << ":\n";
        for (const auto& page : x.getPages()) {
            page.printObjects();
        }
    }
}
} // namespace sru::pdf