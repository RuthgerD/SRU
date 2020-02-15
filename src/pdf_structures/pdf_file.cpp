#include "pdf_file.h"
#include "../util/qpdf_binding.h"
#include "pdf_page.h"
#include <future>
#include <utility>
#include <vector>

namespace sru::pdf {
PdfFile::PdfFile(const std::string& raw, std::filesystem::path path) : raw{""}, path{path} {
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
    real_pages = total_pages;
}
auto operator==(const sru::pdf::PdfPage& a, const sru::pdf::PdfPage& b) noexcept -> bool { return &a == &b; }
auto operator!=(const sru::pdf::PdfPage& a, const sru::pdf::PdfPage& b) noexcept -> bool { return &a != &b; }
auto PdfFile::getPages() const -> const std::vector<std::pair<unsigned int, PdfPage>>& { return pages; }

auto PdfFile::getPage(unsigned int page_no) const -> const std::pair<unsigned int, PdfPage>& {
    if (auto tmp = std::find_if(pages.begin(), pages.end(), [page_no](const auto& x) { return x.first == page_no; }); tmp != pages.end()) {
        return *tmp;
    }
    return pages.back();
}
auto PdfFile::getMarkedObjects(int id) -> std::vector<std::reference_wrapper<sru::pdf::StringObject>> {
    std::vector<std::reference_wrapper<sru::pdf::StringObject>> total{};
    for (auto& page : pages) {
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
    if (auto it = std::find_if(pages.begin(), pages.end(), [&](const auto& x) { return x.second == page; }); it != pages.end()) {
        if (sru::qpdf::delete_page(*this, it->first)) {
            pages.erase(it);
            std::for_each(it, pages.end(), [](auto& x) { x.first -= 1; });
            raw = *sru::util::QFileRead(path);
            return true;
        }
    }
    return false;
}
auto PdfFile::insertPage(PdfPage new_page, unsigned int new_page_no) -> void {
    // maybe assert?
    if (new_page_no > pages.size()) {
        new_page_no = pages.size();
    }
    for (auto& x : pages) {
        if (x.first >= new_page_no) {
            x.first += 1;
        }
    }
    pages.emplace_back(new_page_no, std::move(new_page));
    ++total_pages;
}
auto PdfFile::getRaw() -> std::string {
    // TODO: possible performance issues:
    // TODO: * Dont overwrite pages if they are unchanged
    // TODO: * Dont scan every loop and keep an offset instead
    if (total_pages - real_pages > 0) {
        sru::qpdf::increase_size(*this, total_pages - real_pages);
        real_pages = total_pages;
    }

    auto real_raw = *sru::util::QFileRead(path);
    for (unsigned int j = 0; j < total_pages; ++j) {
        auto sv = std::string_view{real_raw};
        if (auto page_matches_opt = sru::util::re_search(sru::util::page_match_key, sv); page_matches_opt) {
            auto& matches = *page_matches_opt;
            auto& match = matches[j];
            if (auto page = std::find_if(pages.begin(), pages.end(), [&](const auto& x) { return x.first == j; }); page != pages.end()) {
                real_raw.replace(match[2].data() - sv.data(), match[2].size(), page->second.getRaw());
            }
        }
    }
    return real_raw;
}
} // namespace sru::pdf