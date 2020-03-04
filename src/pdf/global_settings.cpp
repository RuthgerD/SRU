#include "global_settings.h"

namespace sru::pdf {
int DATE_PROVIDER;
std::string qpdf_bin;
std::string dcmtk_bin;
int dcmtk_name_obj;
int dcmtk_surname_obj;
int dcmtk_id_obj;
int dcmtk_birth_obj;
int dcmtk_sex_obj;
std::filesystem::path base_path;
size_t base_size;
std::filesystem::path graph_path;
} // namespace sru::pdf