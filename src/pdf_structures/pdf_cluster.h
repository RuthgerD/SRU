#pragma once
#include "../util/qpdf_binding.h"
#include "../util/util.h"
#include "pdf_file.h"
#include "rapidjson/document.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace sru::pdf {
class PdfCluster {

    rapidjson::Document config;
    sru::util::Qpdf qpdf;
    std::vector<sru::pdf::PdfFile> pdf_files;

  public:
    PdfCluster(std::vector<std::filesystem::path> pdf_file_paths,
               sru::util::Qpdf qpdf, std::string config_)
        : qpdf{qpdf} {
        config.Parse(config_);
        for (auto &path : pdf_file_paths) {
            auto deflated = qpdf.decompress(path);
            pdf_files.emplace_back(deflated, config["pdf_file"]);
            std::cout << deflated.generic_string() << "\n";
        }
    };

    const rapidjson::Document &getConfig() const;
};
} // namespace sru::pdf