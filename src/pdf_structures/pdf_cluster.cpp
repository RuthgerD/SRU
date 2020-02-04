#include "pdf_cluster.h"

namespace sru::pdf {
PdfCluster::PdfCluster(std::vector<std::filesystem::path> pdf_file_paths, sru::util::Qpdf qpdf) : qpdf{qpdf} {
    std::vector<std::future<PdfFile>> result;
    // TODO: Use current thread as well :)
    for (const auto& path : pdf_file_paths) {
        result.push_back(std::async([&]() {
            const auto deflated = qpdf.decompress(path);
            // std::cout << deflated.generic_string() << "\n";
            return PdfFile{deflated};
        }));
    }
    int index = 0;
    for (auto& x : result) {
        ++index;
        auto tmp = x.get();
        std::cout << "Page " << index << ":\n";
        for (const auto& page : tmp.getPages()) {
            page.printObjects();
        }
        pdf_files.push_back(std::move(tmp)); // move
    }
}
} // namespace sru::pdf