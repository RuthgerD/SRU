#include "pdf_file.h"
#include "../util/qpdf_binding.h"
#include "pdf_page.h"
#include <utility>
#include <vector>

namespace sru::pdf {
PdfFile::PdfFile(const std::string& raw, std::filesystem::path path) : raw{raw}, path{path} {
    total_pages = 0;
    if (auto page_matches = sru::util::re_search(sru::util::page_match_key, raw); page_matches) {
        for (auto page_match : *page_matches) {
            for (const auto& pconf : sru::pdf::PageConfigPool) {
                if (sru::util::re_match(pconf.regex_id, page_match.at(2))) {
                    pages.emplace_back(std::pair{total_pages, sru::pdf::PdfPage{std::move(std::string{page_match.at(2)}), pconf}})
                        .second.indexObjects();
                    break;
                }
            }
            ++total_pages;
        }
    }
}
bool operator==(const sru::pdf::PdfPage& a, const sru::pdf::PdfPage& b) noexcept { return &a == &b; }
bool operator!=(const sru::pdf::PdfPage& a, const sru::pdf::PdfPage& b) noexcept { return &a != &b; }
auto PdfFile::getPages() const -> const std::vector<std::pair<unsigned int, PdfPage>>& { return pages; }
auto PdfFile::getMarkedObjects(int id) const -> std::vector<std::reference_wrapper<sru::pdf::StringObject>> {
    std::vector<std::reference_wrapper<sru::pdf::StringObject>> total{};
    for (const auto& page : pages) {
        auto tmp = page.second.getMarkedObjects(id);
        total.insert(total.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
    }
    return total;
}
auto PdfFile::getPath() const -> const std::filesystem::path& { return path; }
auto PdfFile::getPageCount() const -> unsigned int { return total_pages; }
auto PdfFile::deletePage(unsigned int page_no) -> bool {
    if (auto tmp = std::find_if(pages.begin(), pages.end(), [page_no](const auto& x) { return x.first == page_no - 1; }); tmp != pages.end()) {
        return deletePage(tmp->second);
    }
    return false;
}
auto PdfFile::deletePage(const sru::pdf::PdfPage& page) -> bool {
    auto it = std::find_if(pages.begin(), pages.end(), [&](const auto& x) { return x.second == page; });
    if (it != pages.end()) {
        if (sru::qpdf::delete_page(*this, it->first)) {
            pages.erase(it);
            raw = *sru::util::QFileRead(path);
            return true;
        }
    }
    return false;
}
auto PdfFile::getRaw() const -> const std::string& { return raw; }
} // namespace sru::pdf