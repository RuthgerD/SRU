#include "pdf_file.h"
#include "../util/re_accel.h"
#include "../util/qpdf_binding.h"
#include "object_config.h"
#include "pdf_page.h"
#include <future>
#include <utility>
#include <vector>

namespace sru::pdf {
PdfFile::PdfFile(const std::string& raw, std::filesystem::path path) : raw_{""}, path_{path} {
    total_pages_ = 0;
    if (auto page_matches = sru::re::re_search(sru::re::page_match_key, raw); page_matches) {
        for (auto page_match : *page_matches) {
            const PageConfig* best_config = nullptr;
            for (const auto& pconf : sru::pdf::PageConfigPool) {
                if (sru::re::re_match(pconf.regex_id, page_match[2])) {
                    if (!best_config || best_config->priority > pconf.priority) {
                        best_config = &pconf;
                    }
                    if (pconf.priority == 0) {
                        break;
                    }
                }
            }
            if (best_config) {
                pages_.emplace_back(std::pair{total_pages_, sru::pdf::PdfPage{std::move(std::string{page_match[2]}), *best_config}})
                    .second.indexObjects();
            }
            ++total_pages_;
        }
    }
    real_pages_ = total_pages_;
}
auto operator==(const sru::pdf::PdfPage& a, const sru::pdf::PdfPage& b) noexcept -> bool { return &a == &b; }
auto operator!=(const sru::pdf::PdfPage& a, const sru::pdf::PdfPage& b) noexcept -> bool { return &a != &b; }
auto PdfFile::getPages() const -> const std::vector<std::pair<size_t, PdfPage>>& { return pages_; }
auto PdfFile::getPages() -> std::vector<std::pair<size_t, PdfPage>>& { return pages_; }

// TODO: duplicate code also maybe not needed
auto PdfFile::getPage(size_t page_no) -> PdfPage& {
    if (auto tmp = std::find_if(pages_.begin(), pages_.end(), [page_no](const auto& x) { return x.first == page_no; }); tmp != pages_.end()) {
        return tmp->second;
    }
    return pages_.back().second;
}
auto PdfFile::getPage(size_t page_no) const -> const PdfPage& {
    if (auto tmp = std::find_if(pages_.begin(), pages_.end(), [page_no](const auto& x) { return x.first == page_no; }); tmp != pages_.end()) {
        return tmp->second;
    }
    return pages_.back().second;
}
auto PdfFile::getMarkedObjects(int id) -> std::vector<std::pair<int, std::vector<offset>>> {
    std::vector<std::pair<int, std::vector<offset>>> ret;
    for (auto& [page_no, page] : pages_) {
        auto mkrd = page.getMarkedObjects(id);
        if (!mkrd.empty()){
            ret.emplace_back(page_no, std::move(mkrd));
        }
    }
    return ret;
}
auto PdfFile::getPath() const -> const std::filesystem::path& { return path_; }
auto PdfFile::getPageCount() const -> size_t { return total_pages_; }
auto PdfFile::getRealPageCount() const -> size_t { return real_pages_; }
auto PdfFile::deletePage(const sru::pdf::PdfPage& page) -> bool {
    if (auto it = std::find_if(pages_.begin(), pages_.end(), [&](const auto& x) { return x.second == page; }); it != pages_.end()) {
        const auto& comp_no = it->first;
        for (auto& [page_no, page_] : pages_) {
            if (page_no > comp_no) {
                page_no -= 1;
            }
        }
        pages_.erase(it);
        --total_pages_;
        return true;
    }
    return false;
}
auto PdfFile::insertPage(PdfPage new_page, size_t new_page_no) -> void { insertPages({std::move(new_page)}, new_page_no); }
auto PdfFile::insertPages(std::vector<PdfPage> new_pages, size_t new_page_no) -> void {
    // maybe assert?
    if (new_page_no > pages_.size()) {
        new_page_no = pages_.size();
    }
    for (auto& x : pages_) {
        if (x.first >= new_page_no) {
            x.first += new_pages.size();
        }
    }
    pages_.reserve(new_pages.size());
    total_pages_ += new_pages.size();
    for (size_t i = 0; i < new_pages.size(); ++i) {
        pages_.emplace_back(new_page_no + i, std::move(new_pages[i]));
    }
}
auto PdfFile::getRaw() -> std::string {
    // TODO: * Dont scan every loop and keep an offset instead
    if (total_pages_ != real_pages_) {
        sru::qpdf::change_size(*this, (int)total_pages_ - (int)real_pages_);
        real_pages_ = total_pages_;
    }

    auto real_raw = *sru::util::QFileRead(path_);
    for (size_t j = 0; j < total_pages_; ++j) {
        auto sv = std::string_view{real_raw};
        if (auto page_matches_opt = sru::re::re_search(sru::re::page_match_key, sv); page_matches_opt) {
            auto& matches = *page_matches_opt;
            auto& match = matches[j];
            if (auto page = std::find_if(pages_.begin(), pages_.end(), [&](const auto& x) { return x.first == j; }); page != pages_.end()) {
                real_raw.replace(match[2].data() - sv.data(), match[2].size(), page->second.getRaw());
            }
        }
    }
    return real_raw;
}
} // namespace sru::pdf