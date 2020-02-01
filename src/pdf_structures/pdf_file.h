#pragma once
#include "../util/re_accel.h"
#include "../util/util.h"
#include "pdf_page.h"
#include "string_obj.h"
#include <boost/regex.hpp>
#include <ctll.hpp>
#include <ctre.hpp>
#include <filesystem>
#include <iostream>
#include <re2.h>
#include <string>
#include <vector>

namespace sru::pdf {
class PdfFile {

    std::filesystem::path path;
    std::vector<PdfPage> pages;

  public:
    PdfFile(std::filesystem::path path)
        : path{path} {
        auto contents = sru::util::QFileRead(path);
        int page_no = 0;
        if (auto page_matches =
                sru::util::regex_accel[sru::util::page_match_key](
                    contents.value())) {
            for (auto page_match : page_matches.value()) {
                ++page_no;
                auto page_content = std::string{page_match.at(2)};

                pages.emplace_back(page_content);
            }
        }
    }

    const std::filesystem::path &getPath() const;
    const std::vector<PdfPage> &getPages() const;
};
}; // namespace sru::pdf