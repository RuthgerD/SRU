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
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <re2.h>
#include <string>
#include <vector>

namespace sru::pdf {
class PdfFile {

    std::filesystem::path path;
    rapidjson::Value &config;
    std::vector<PdfPage> pages;

  public:
    PdfFile(std::filesystem::path path, rapidjson::Value &config)
        : path{path}, config{config} {
        auto contents = sru::util::QFileRead(path);
        int page_no = 0;
        if (auto page_matches =
                sru::util::regex_accel[sru::util::page_match_key](
                    contents.value())) {
            for (auto page_match : page_matches.value()) {
                ++page_no;
                // std::cout << "page: " << page_no << "\n";
                auto page_content = std::string{page_match.at(2)};
                // config finder :)
                auto &page_config = config["page_"];
                for (auto &p : config.GetObject()) {
                  std::cout << "bruh";
                    // if (!std::strcmp(p.name.GetString(), "page_")) {
                    //     std::cout << p.name.GetString();
                    //     std::cout << config[p.name.GetString()]["regex_id"]
                    //                      .GetString()
                    //               << "\n";
                    //     auto regex_id = boost::regex{
                    //         config[p.name.GetString()]["regex_id"].GetString()};
                    //     if (boost::regex_search(page_content, regex_id)) {
                    //         page_config = config[p.name.GetString()];
                    //         std::cout << "Found config!: " << p.name.GetString()
                    //                   << "\n";
                    //     }
                    // }
                }

                // if (page_no == 1)
                pages.emplace_back(page_content, page_config);
            }
        }
    }

    const std::filesystem::path &getPath() const;
    const std::vector<PdfPage> &getPages() const;
};
}; // namespace sru::pdf