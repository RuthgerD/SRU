#include "qpdf_binding.h"
#include "util.h"
#include <filesystem>
#include <string>
#include <string_view>

namespace sru::qpdf {

class Qpdf {
    std::filesystem::path cache_path;
#ifdef __linux__
    const std::string bin{"qpdf"};
#else
    const std::string bin{"qpdf/qpdf.exe"};
#endif
  public:
    Qpdf() : cache_path{std::filesystem::current_path()} { cache_path.append(".qpdf_cache"); }
    [[nodiscard]] auto get_bin() const -> const std::string& { return bin; }
    [[nodiscard]] auto get_cache_path() const -> const std::filesystem::path& {
        if (!std::filesystem::is_directory(cache_path)) {
            std::filesystem::create_directories(cache_path);
        }
        return cache_path;
    }
    auto set_cache_path(std::filesystem::path& path) -> bool {
        // TODO: add checks
        cache_path = path;
        return true;
    }
} qpdf_settings{};

auto set_cache_path(std::filesystem::path& path) -> void { qpdf_settings.set_cache_path(path); }

auto decompress(const std::filesystem::path& pdf_file) -> std::optional<std::filesystem::path> {
    const auto abs_pdf_file = std::filesystem::absolute(pdf_file.lexically_normal());
    auto abs_cache_path = std::filesystem::absolute(qpdf_settings.get_cache_path());
    abs_cache_path.append(pdf_file.filename().generic_string());

    auto command = qpdf_settings.get_bin() + " --stream-data=uncompress \"" + abs_pdf_file.generic_string() + "\" -- \"" +
                   abs_cache_path.generic_string() + "\"";

    sru::util::cmd(command);

    return abs_cache_path;
}

auto delete_page(const std::filesystem::path& pdf_file, unsigned int page_no) -> bool {
    const auto abs_pdf_file = std::filesystem::absolute(pdf_file.lexically_normal());
    auto abs_cache_path = std::filesystem::absolute(qpdf_settings.get_cache_path());
    abs_cache_path.append(pdf_file.filename().generic_string());

    auto tmp_path = abs_pdf_file;
    tmp_path.concat("-tmp");
    auto command = qpdf_settings.get_bin() + " --empty " + tmp_path.c_str() + " --stream-data=uncompress --pages \"" + abs_pdf_file.generic_string() +
                   "\" 1-" + std::to_string(page_no - 1) + +" \"" + abs_pdf_file.generic_string() + "\" " + std::to_string(page_no + 1) + "-z --";

    sru::util::cmd(command);

    // TODO: fix unsafe
    std::filesystem::remove(abs_pdf_file);
    std::filesystem::rename(tmp_path, abs_pdf_file);

    return true;
}

} // namespace sru::qpdf
