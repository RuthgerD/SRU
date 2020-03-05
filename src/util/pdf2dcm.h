#pragma once
#include "util.h"

namespace sru::dcmtk {
    struct PatientData {
        int patient_id;
        std::string patient_name;
        std::chrono::system_clock::time_point birth_date;
        std::chrono::system_clock::time_point start_date;
        bool sex;
        std::string company;
        std::string device;
        auto to_command() -> std::string;
    };
    auto set_bin(std::string bin) -> void;
    auto convert(const std::filesystem::path& pdf_file, const std::filesystem::path& out, PatientData data) -> bool;
} // namespace sru::dcmtk