#pragma once
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
#include <vector>

static constexpr auto page_match = ctll::fixed_string{
    R"(([0-9]*?) . obj\n<< /BBox.*?>>\nstream\n((\n|.)*?)\nendstream\nendobj)"};

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
        for (auto page_match : ctre::range<page_match>(contents.value())) {
            ++page_no;
            // std::cout << "page: " << page_no << "\n";
            auto page_content = page_match.get<2>().to_string();
            // config finder :)
            auto &page_config = config["page_"];
            // for (auto &p : config.GetObject()) {
            //     if (std::strcmp(p.name.GetString(), "page_")) {
            //         std::cout << p.name.GetString() << "\n";
            //         auto regex_id = boost::regex{
            //             config[p.name.GetString()]["regex_id"].GetString()};
            //         if (boost::regex_search(page_content, regex_id)) {
            //             page_config = config[p.name.GetString()];
            //             std::cout << "Found config!: " << p.name.GetString()
            //                       << "\n";
            //         }
            //     }
            // }
            //
            pages.emplace_back(page_content, page_config);
        }
    }

    const std::filesystem::path &getPath() const;
    const std::vector<PdfPage> &getPages() const;
};
}; // namespace sru::pdf