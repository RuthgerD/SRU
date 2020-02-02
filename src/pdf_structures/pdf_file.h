#pragma once
#include "../util/re_accel.h"
#include "../util/util.h"
#include "anchor_config.h"
#include "pdf_page.h"
#include "string_obj.h"
#include <boost/regex.hpp>
#include <chrono>
#include <ctll.hpp>
#include <ctre.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace sru::pdf {
static float timepool = 0;
class PdfFile {

    std::filesystem::path path;
    std::vector<PdfPage> pages;

  public:
    PdfFile(std::filesystem::path path) : path{path} {
        auto contents = sru::util::QFileRead(path);
        int page_no = 0;
        if (auto page_matches = sru::util::re_search(sru::util::page_match_key,
                                                     contents.value())) {
            for (auto page_match : page_matches.value()) {
                ++page_no;
                for (const auto pconf : sru::pdf::PageConfigPool) {
                    if (auto result = sru::util::re_search(pconf.regex_id,
                                                           page_match.at(2))) {
                        const auto start = std::chrono::steady_clock::now();
                        pages.emplace_back(page_match.at(2), pconf)
                            .indexObjects();
                        const auto end = std::chrono::steady_clock::now();
                        timepool += (std::chrono::duration_cast<
                                         std::chrono::microseconds>(end - start)
                                         .count()) /
                                    1000000.0;
                        break;
                    }
                }
            }
        }
        std::cout << "Pdf file creation (sec) = " << timepool << "\n";
    }

    const std::filesystem::path &getPath() const;
    const std::vector<PdfPage> &getPages() const;
};
}; // namespace sru::pdf