#pragma once
#include "../util/qpdf_binding.h"
#include "../util/util.h"
#include "anchor_config.h"
#include "calc_config.h"
#include "object_config.h"
#include "pdf_file.h"
#include "../util/pdf2dcm.h"
#include <boost/functional/hash.hpp>
#include <ctime>
#include <filesystem>
#include <future>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace sru::pdf {
class PdfCluster {

    std::vector<sru::pdf::PdfFile> pdf_files_;

  public:
    explicit PdfCluster(std::vector<std::filesystem::path> pdf_file_paths);
    [[nodiscard]] static auto getMarkedObjects(int id, std::vector<PdfFile>& files) -> std::vector<StringObject>;
    [[nodiscard]] auto calculateObject(const ObjectConfig& object_conf) -> std::vector<StringObject>;
    auto calculateObject(const ObjectConfig& object_conf, const CalcConfig& calc_config) -> std::vector<std::string>;
    auto calccalc(const CalcConfig& cc, const std::vector<std::string>& contents, std::string reference) -> std::vector<std::string>;
    auto export_merged() -> void;
    static auto refreshNumbering(PdfFile& file) -> void;
    auto GenDicom() -> sru::dcmtk::PatientData;
};
} // namespace sru::pdf