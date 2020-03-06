#include "pdf_file.h"
#include "../util/qpdf_binding.h"
#include "../util/re_accel.h"
#include "object_config.h"
#include "pdf_page.h"
#include <utility>
#include <vector>

namespace sru::pdf {
PdfFile::PdfFile(const std::string& raw, std::filesystem::path path) : raw_{""}, path_{std::move(path)} {

    total_pages_ = 0;
    if (auto page_matches = sru::re::re_search(sru::re::r41_key, raw); page_matches) {
        for (auto page_match : *page_matches) {
            const PageConfig* best_config = nullptr;
            for (const auto& pconf : sru::pdf::PageConfigPool) {
                if (sru::re::re_match(pconf.regex_id, page_match[1])) {
                    if (!best_config || best_config->priority > pconf.priority) {
                        best_config = &pconf;
                    }
                    if (pconf.priority == 0) {
                        break;
                    }
                }
            }
            if (best_config) {
                pages_.emplace_back(std::pair{total_pages_, sru::pdf::PdfPage{std::string{page_match[1]}, *best_config}}).second.indexObjects();
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
auto PdfFile::getMarkedObjects(int id) -> std::vector<std::pair<int, std::vector<Offset>>> {
    std::vector<std::pair<int, std::vector<Offset>>> ret;
    for (auto& [page_no, page] : pages_) {
        auto mkrd = page.getMarkedObjects(id);
        if (!mkrd.empty()) {
            ret.emplace_back(page_no, std::move(mkrd));
        }
    }
    return ret;
}
auto PdfFile::getPath() const -> const std::filesystem::path& { return path_; }
auto PdfFile::getPageCount() const -> size_t { return total_pages_; }
auto PdfFile::getRealPageCount() const -> size_t { return real_pages_; }
auto PdfFile::deletePage(size_t page_no) -> void {
    std::vector<size_t> tmp{page_no};
    deletePages(tmp);
}
auto PdfFile::deletePages(std::vector<size_t>& page_nos) -> void {
    std::sort(page_nos.begin(), page_nos.end());
    page_nos.erase(std::unique(page_nos.begin(), page_nos.end()), page_nos.end());
    // TODO: std::erase_if for c++20
    pages_.erase(std::remove_if(pages_.begin(), pages_.end(),
                                [&](const auto& page) {
                                    for (auto& page_no : page_nos) {
                                        if (page_no == page.first) {
                                            --total_pages_;
                                            return true;
                                        }
                                    }
                                    return false;
                                }),
                 pages_.end());

    for (auto& [key, val] : pages_) {
        key -= std::distance(page_nos.begin(), std::upper_bound(page_nos.begin(), page_nos.end(), key));
    }
}
auto PdfFile::insertPage(PdfPage& new_page, size_t new_page_no) -> void {
    std::vector<PdfPage> tmp{std::move(new_page)};
    insertPages(tmp, new_page_no);
}
auto PdfFile::insertPages(std::vector<PdfPage>& new_pages, size_t new_page_no) -> void {
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
auto PdfFile::appendPage(PdfPage& new_page) -> void { insertPage(new_page, pages_.size()); }
auto PdfFile::appendPages(std::vector<PdfPage>& new_pages) -> void { insertPages(new_pages, pages_.size()); }
auto PdfFile::write(std::ofstream& os, const std::filesystem::path& base) -> void {
    auto t1 = std::chrono::steady_clock::now().time_since_epoch();
    if (total_pages_ == 0) {
        return;
    }

    std::string real_raw;
    if (auto real_raw_opt = sru::util::QFileRead(base); real_raw_opt) {
        real_raw = std::move(*real_raw_opt);
    } else {
        return;
    }
    std::vector<std::string_view> sorted_pages;
    sorted_pages.resize(total_pages_);
    for (size_t j = 0; j < total_pages_; ++j) {
        if (auto page = std::find_if(pages_.begin(), pages_.end(), [&](const auto& x) { return x.first == j; }); page != pages_.end()) {
            sorted_pages[j] = page->second.getRaw();
        }
    }
    auto sv = std::string_view{real_raw};
    auto lengths = *sru::re::re_search("/XObject /Length (\\d+)", sv);
    long length_offset = 0;
    for (size_t j = 0; j < total_pages_; ++j) {
        auto& old_length = lengths[j][1];
        auto new_string = std::to_string(sorted_pages[j].size());
        real_raw.replace(std::distance(sv.begin(), old_length.begin()) + length_offset, old_length.size(), new_string);
        length_offset += (long)new_string.size() - (long)old_length.size();
    }
    sv = std::string_view{real_raw};

    std::vector<std::vector<std::string_view>> page_matches;
    if (auto page_matches_opt = sru::re::re_search(sru::re::r41_key, sv); page_matches_opt) {
        page_matches = std::move(*page_matches_opt);
    } else {
        return;
    }

    std::vector<std::pair<int, int>> offsets_and_lengths;
    std::transform(page_matches.begin(), page_matches.end(), std::back_inserter(offsets_and_lengths), [&](const auto& vec_sv) {
        auto& content_view = vec_sv[1];

        return std::pair{std::distance(sv.begin(), content_view.begin()), content_view.size()};
    });
    sru::util::sink(std::move(page_matches));

    auto raw_offset = 0;
    std::vector<unsigned long> xref;
    for (size_t j = 0; j < total_pages_; ++j) {
        const auto& [offset, length] = offsets_and_lengths[j];
        const auto view_length = sorted_pages[j].length();
        const auto block_length = offset - raw_offset;

        auto view = std::string_view{real_raw.data() + raw_offset, (unsigned long)block_length};
        auto objs = sru::re::re_search(R"(\d+ \d+ obj\n)", view);
        for (auto& t : *objs) {
            xref.emplace_back(os.tellp() + std::distance(view.begin(), t.front().begin()));
        }
        auto page_length = sru::re::re_search(R"(Length (\d+))", view)->back();
        auto new_page_length = std::to_string(view_length);
        os.write(real_raw.data() + raw_offset, block_length);
        os.write(sorted_pages[j].data(), view_length);
        raw_offset = offset + length;
    }
    const auto& [offset, length] = offsets_and_lengths.back();
    auto view = std::string_view{&real_raw[offset + length], (unsigned long)std::distance(real_raw.begin() + offset + length, real_raw.end())};
    auto objs = sru::re::re_search(R"(\d+ \d+ obj\n)", view);
    for (auto& t : *objs) {
        xref.emplace_back(os.tellp() + std::distance(view.begin(), t.front().begin()));
    }
    std::string new_xref = "\nxref\n";
    new_xref += std::string{"0 "} + std::to_string(xref.size()+1) + "\n" + "0000000000 65535 f\n";
    for (auto& x : xref) {
        std::string tmp = std::to_string(x);
        sru::util::zfill(tmp, 10);
        new_xref += tmp + " 00000 n\n";
    }
    auto last_view = std::string_view{&real_raw[offset + length], (unsigned long)std::distance(real_raw.begin() + offset + length, real_raw.end())};
    auto start_xref = last_view.rfind("\nxref\n");
    std::string id = fmt::format("{:x}", t1.count());
    auto t2 = std::chrono::steady_clock::now().time_since_epoch() - t1;
    id += fmt::format("{:x}", t2.count());
    id += id + id;
    new_xref += "trailer << /Info 2 0 R /Root 1 0 R /Size " + std::to_string(xref.size() + 1) +
                " /ID [<"+id.substr(0,32)+"><"+id.substr(0,32)+">] >>\n";
    new_xref += "startxref\n" + std::to_string((unsigned long)start_xref + os.tellp() + 1) + "\n";
    new_xref += "%%EOF\n";
    os.write(last_view.data(), start_xref);
    os.write(new_xref.data(), new_xref.size());
    os.flush();
    os.close();
}
} // namespace sru::pdf