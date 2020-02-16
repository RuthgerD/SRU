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
            const PageConfig* best_config = nullptr;
            for (const auto& pconf : sru::pdf::PageConfigPool) {
                if (sru::util::re_match(pconf.regex_id, page_match[2])) {
                    if (!best_config || best_config->priority > pconf.priority) {
                        best_config = &pconf;
                    }
                    if (pconf.priority == 0) {
                        break;
                    }
                }
            }
            if (best_config) {
                pages.emplace_back(std::pair{total_pages, sru::pdf::PdfPage{std::move(std::string{page_match[2]}), *best_config}})
                    .second.indexObjects();
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
auto PdfFile::getPageCount() const -> int { return total_pages; }
auto PdfFile::deletePage(unsigned int page_no) -> bool {
    if (auto tmp = std::find_if(pages.begin(), pages.end(), [page_no](const auto& x) { return x.first == page_no; }); tmp != pages.end()) {
        return deletePage(tmp->second);
    }
    return false;
}
auto PdfFile::deletePage(const sru::pdf::PdfPage& page) -> bool {
    if (auto it = std::find_if(pages.begin(), pages.end(), [&](const auto& x) { return x.second == page; }); it != pages.end()) {
        for (auto& x : pages) {
            if (x.first > it->first) {
                x.first -= 1;
            }
        }
        pages.erase(it);
        --total_pages;
        return true;
    }
    return false;
}
auto PdfFile::insertPage(PdfPage new_page, unsigned int new_page_no) -> void { insertPages({std::move(new_page)}, new_page_no); }
auto PdfFile::insertPages(std::vector<std::pair<unsigned int, PdfPage>> new_pages, unsigned int new_page_no) -> void {
    std::vector<PdfPage> new_pages_unpacked;
    pages.reserve(new_pages.size());
    for (auto& page : new_pages) {
        new_pages_unpacked.push_back(std::move(page.second));
    }
    insertPages(std::move(new_pages_unpacked), new_page_no);
}
auto PdfFile::insertPages(std::vector<PdfPage> new_pages, unsigned int new_page_no) -> void {
    // maybe assert?
    if (new_page_no > pages.size()) {
        new_page_no = pages.size();
    }
    for (auto& x : pages) {
        if (x.first >= new_page_no) {
            x.first += new_pages.size();
        }
    }
    pages.reserve(new_pages.size());
    total_pages += new_pages.size();
    for (int i = 0; i < new_pages.size(); ++i) {
        pages.emplace_back(new_page_no + i, std::move(new_pages[i]));
    }
}
auto PdfFile::getRaw() -> std::string {
    // TODO: possible performance issues:
    // TODO: * Dont overwrite pages if they are unchanged
    // TODO: * Dont scan every loop and keep an offset instead
    if (total_pages != real_pages) {
        sru::qpdf::change_size(*this, (int)total_pages - (int)real_pages);
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
    return std::move(real_raw);
}
} // namespace sru::pdf