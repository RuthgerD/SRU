#include "pdf2dcm.h"

namespace sru::dcmtk {
class Dcm {
    std::string bin;

  public:
    Dcm()
        : bin{
#ifdef __linux__
        "pdf2dcm"
#else
        "dcmtk\\pdf2dcm.exe"
#endif
    } {
    }
    [[nodiscard]] auto get_bin() const -> const std::string& { return bin; }
    auto set_bin(std::filesystem::path& path) -> bool {
        bin = path;
        return true;
    }
} dcm_settings{};
auto PatientData::to_command() -> std::string {
    std::string data_set;
    std::string date_of_birth = "00000000";
    if (auto opt = sru::util::strftime(birth_date, "%Y%m%d"); opt) {
        date_of_birth = std::move(*opt);
    }
    std::string StudyDate = "00000000";
    if (auto opt = sru::util::strftime(start_date, "%Y%m%d"); opt) {
        StudyDate = std::move(*opt);
    }
    std::string StudyTime = "000000";
    if (auto opt = sru::util::strftime(start_date, "%H%M%S"); opt) {
        StudyTime = std::move(*opt);
    }

    data_set += std::string{" +pn "} + "\"" + patient_name + "\"" + " +pb " + date_of_birth + " +ps " + (sex ? "M" : "F") + " +pi " + std::to_string(patient_id);
    data_set += std::string{" -k "} + "0008,0060=\"SC\"" + " -k " + "0008,0008=\"DERIVED\\SECONDARY\"" + " -k " + "0008,0020=\"" + StudyDate + "\"" + " -k " + "0008,0021=\"" + StudyDate + "\"" + " -k " + "0008,0023=\"" + StudyDate + "\"" + " -k " + "0008,0030=\"" + StudyTime + "\"" + " -k " + "0008,0031=\"" + StudyTime + "\"" + " -k " + "0008,0033=\"" + StudyTime + "\"" + " -k " + "0008,0070=\"" + company + "\"" + " -k " + "0008,1030=\"" + device + "\"" + " -k " + "0008,103E=\"" + device + "\"" + " -k " + "0008,1090=\"" + company + "\"" + " -k " + "0018,1030=\"" + device + "\"" + " -k " + "0028,0010=\"1\"" + " -k " + "0028,0011=\"1\"" + " -k " + "0028,2110=\"01\"" + " -k " + "0040,0254=\"" + device + "\"" + " -k " + "0002,0000=\"194\"";

    return data_set;
}
auto convert(const std::filesystem::path& pdf_file, const std::filesystem::path& out, PatientData data) -> bool {
    std::string command = dcm_settings.get_bin() + data.to_command() + " \"" + pdf_file.generic_string() + "\" \"" + out.generic_string() + "\"";
    sru::util::cmd(command);
    return std::filesystem::exists(out);
}
}