#include "pdf_file.h"
#include "pdf_page.h"
#include <vector>

namespace sru::pdf {
PdfFile::PdfFile(const std::filesystem::path& path) : path{path} {
    auto contents = sru::util::QFileRead(path);
    int page_no = 0;
    if (auto page_matches = sru::util::re_search(sru::util::page_match_key, contents.value()); page_matches) {
        for (auto page_match : *page_matches) {
            ++page_no;
            for (const auto& pconf : sru::pdf::PageConfigPool) {
                if (sru::util::re_match(pconf.regex_id, page_match.at(2))) {
                    pages.emplace_back(std::move(std::string{page_match.at(2)}), pconf).indexObjects();
                    break;
                }
            }
        }
    }
}
const std::vector<sru::pdf::PdfPage>& PdfFile::getPages() const { return pages; }
std::vector<std::reference_wrapper<sru::pdf::StringObject>> PdfFile::getMarkedObjects(int id) const {
    std::vector<std::reference_wrapper<sru::pdf::StringObject>> total{};
    for (const auto& page : pages) {
        auto tmp = page.getMarkedObjects(id);

        total.insert(total.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
    }
    return total;
}

    const std::filesystem::path &PdfFile::getPath() const {
        return path;
    }
} // namespace sru::pdf