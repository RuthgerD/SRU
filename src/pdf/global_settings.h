#pragma once
#include <filesystem>
namespace sru::pdf {
extern int DATE_PROVIDER;
extern std::string qpdf_bin;
extern std::string dcmtk_bin;
extern std::string dcmtk_company;
extern std::string dcmtk_device;
extern int dcmtk_name_obj;
extern int dcmtk_surname_obj;
extern int dcmtk_id_obj;
extern int dcmtk_birth_obj;
extern int dcmtk_sex_obj;
extern std::filesystem::path base_path;
extern size_t base_size;
extern std::filesystem::path graph_path;
} // namespace sru::pdf