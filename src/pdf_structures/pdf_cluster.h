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
#include <boost/functional/hash.hpp>

namespace sru::pdf {
class PdfCluster {

    std::vector<sru::pdf::PdfFile> pdf_files_;

  public:
    explicit PdfCluster(std::vector<std::filesystem::path> pdf_file_paths);
    [[nodiscard]] auto getMarkedObjects(int id, std::vector<PdfFile>& files) -> std::vector<StringObject>;
    [[nodiscard]] auto calculateObject(const ObjectConfig& object_conf, const std::vector<StringObject>& total_objects) -> std::vector<StringObject>;
    auto exportTest() -> void;
    auto refreshNumbering(PdfFile& file) -> void;
};
} // namespace sru::pdf