#pragma once
#include "../util/qpdf_binding.h"
#include "../util/util.h"
#include "anchor_config.h"
#include "object_config.h"
#include "pdf_file.h"
#include <filesystem>
#include <future>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <ctime>

namespace sru::pdf {
class PdfCluster {

    std::vector<sru::pdf::PdfFile> pdf_files;

  public:
    explicit PdfCluster(std::vector<std::filesystem::path> pdf_file_paths);
    [[nodiscard]] auto getMarkedObjects(int id) -> std::vector<std::reference_wrapper<sru::pdf::StringObject>>;
    auto calculate() -> std::unordered_map<int, std::vector<sru::pdf::StringObject>>;
    auto exportTest() -> void;
};
} // namespace sru::pdf