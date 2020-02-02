#pragma once
#include "../util/qpdf_binding.h"
#include "../util/util.h"
#include "anchor_config.h"
#include "pdf_file.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace sru::pdf {
class PdfCluster {

    sru::util::Qpdf qpdf;
    std::vector<sru::pdf::PdfFile> pdf_files;

  public:
    PdfCluster(std::vector<std::filesystem::path> pdf_file_paths,
               sru::util::Qpdf qpdf);
};
} // namespace sru::pdf