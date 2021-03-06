#include "qpdf_binding.h"
#include "../pdf/pdf_file.h"
#include "util.h"
#include <filesystem>
#include <string>
#include <string_view>

namespace sru::qpdf {

class Qpdf {
    std::filesystem::path cache_path;
    std::string bin;

  public:
    Qpdf()
        : cache_path{std::filesystem::current_path()}, bin{
#ifdef __linux__
                                                           "qpdf"
#else
                                                           "qpdf\\qpdf.exe"
#endif
                                                       } {
        cache_path.append(".qpdf_cache");
    }
    [[nodiscard]] auto get_bin() const -> const std::string& { return bin; }
    [[nodiscard]] auto get_cache_path() const -> const std::filesystem::path& {
        if (!std::filesystem::is_directory(cache_path)) {
            std::filesystem::create_directories(cache_path);
        }
        return cache_path;
    }
    auto set_cache_path(std::filesystem::path& path) -> bool {
        cache_path = path;
        return true;
    }
    auto set_bin(std::string& path) -> bool {
        bin = path;
        return true;
    }
} qpdf_settings{};
auto set_cache_path(std::filesystem::path& path) -> void { qpdf_settings.set_cache_path(path); }
auto set_bin(std::string& path) -> void { qpdf_settings.set_bin(path); };

auto decompress(const std::filesystem::path& pdf_file) -> std::optional<std::filesystem::path> {
    const auto abs_pdf_file = std::filesystem::absolute(pdf_file.lexically_normal());
    auto abs_cache_path = std::filesystem::absolute(qpdf_settings.get_cache_path());
    abs_cache_path.append(pdf_file.filename().generic_string());

    auto command = qpdf_settings.get_bin() + " --stream-data=uncompress \"" + abs_pdf_file.generic_string() + "\" -- \"" +
                   abs_cache_path.generic_string() + "\"";

    sru::util::cmd(command);

    return abs_cache_path;
}
auto compress(const std::filesystem::path& pdf_file) -> bool {
    const auto abs_pdf_file = std::filesystem::absolute(pdf_file.lexically_normal());
    auto retard_cache = abs_pdf_file;
    retard_cache.concat("-compress");
    auto command = qpdf_settings.get_bin() + " --stream-data=compress \"" + abs_pdf_file.generic_string() + "\" -- \"" +
                   retard_cache.generic_string() + "\"";
    sru::util::cmd(command);

    if (std::filesystem::exists(retard_cache)) {
        std::filesystem::remove(abs_pdf_file);
        std::filesystem::rename(retard_cache, abs_pdf_file);
        return true;
    }
    return false;
}
auto validate(const std::filesystem::path& pdf_file) -> bool {
    const auto abs_pdf_file = std::filesystem::absolute(pdf_file.lexically_normal());
    auto retard_cache = abs_pdf_file;
    retard_cache.concat("-compress");
    auto command = qpdf_settings.get_bin() + " --stream-data=preserve \"" + abs_pdf_file.generic_string() + "\" -- \"" +
                   retard_cache.generic_string() + "\"";
    sru::util::cmd(command);

    if (std::filesystem::exists(retard_cache)) {
        std::filesystem::remove(abs_pdf_file);
        std::filesystem::rename(retard_cache, abs_pdf_file);
        return true;
    }
    return false;
}
auto delete_page(const sru::pdf::PdfFile& pdf_file, size_t page_no) -> bool {
    ++page_no;
    const auto abs_pdf_file = std::filesystem::absolute(pdf_file.getPath().lexically_normal());

    auto tmp_path = abs_pdf_file;
    tmp_path.concat("-tmp");

    auto command = qpdf_settings.get_bin() + " --empty " + tmp_path.generic_string() + " --stream-data=preserve " + "--pages ";
    command += abs_pdf_file.generic_string() + " ";
    if (page_no == 1) {
        command += "2-z";
    } else if (page_no == pdf_file.getPageCount()) {
        command += "1-r2";
    } else {
        command += "1-" + std::to_string(page_no - 1) + " ";
        command += abs_pdf_file.generic_string() + " " + std::to_string(page_no + 1) + "-z";
    }
    command += " --";

    sru::util::cmd(command);
    if (std::filesystem::exists(tmp_path)) {
        std::filesystem::remove(abs_pdf_file);
        std::filesystem::rename(tmp_path, abs_pdf_file);

        return true;

    } else {
        return false;
    }
}
auto insert_page(const sru::pdf::PdfFile& pdf_file_out, size_t page_no, const sru::pdf::PdfFile& pdf_file_in, size_t new_page_no) -> bool {
    ++page_no;
    ++new_page_no;
    auto out_path = std::filesystem::absolute(pdf_file_out.getPath().lexically_normal());
    auto in_path = std::filesystem::absolute(pdf_file_in.getPath().lexically_normal());
    auto tmp_path = in_path;
    tmp_path.concat("-tmp");

    std::string page = " " + out_path.generic_string() + " " + std::to_string(page_no) + "-" + std::to_string(page_no);

    std::string command = qpdf_settings.get_bin() + " --empty " + tmp_path.generic_string() + " --stream-data=preserve " + "--pages ";
    if (new_page_no == 1) {
        command += page;
        command += " " + in_path.generic_string() + " 1-z";
    } else if (new_page_no >= pdf_file_in.getPageCount()) {
        command += " " + in_path.generic_string() + " 1-z";
        command += page;
    } else {
        command += " " + in_path.generic_string() + " 1-" + std::to_string(new_page_no - 1);
        command += page;
        command += " " + in_path.generic_string() + " " + std::to_string(new_page_no) + "-z";
    }
    command += " --";
    sru::util::cmd(command);

    if (std::filesystem::exists(tmp_path)) {
        std::filesystem::remove(in_path);
        std::filesystem::rename(tmp_path, in_path);
        return true;

    } else {
        return false;
    }
}
auto increase_size(const std::filesystem::path& path, size_t from, size_t size) -> bool {
    while (size > from) {
        increase_size(path, from, from);
        size -= from;
    }

    const auto abs_pdf_file = std::filesystem::absolute(path.lexically_normal());
    auto retard_cache = abs_pdf_file;
    retard_cache.concat("-tmp" + std::to_string((int)size));
    std::string command{};
    if (size > 0) {
        command = qpdf_settings.get_bin() + " \"" + abs_pdf_file.generic_string() + "\" --stream-data=preserve --pages . 1-z \"" +
                  std::filesystem::relative(abs_pdf_file).generic_string() + "\" 1-" + std::to_string(size) + " -- \"" +
                  retard_cache.generic_string() + "\"";
    } else if (size < 0) {
        command = qpdf_settings.get_bin() + " \"" + abs_pdf_file.generic_string() + "\" --stream-data=preserve --pages . 1-r" +
                  std::to_string(-1 * size + 1) + " -- \"" + retard_cache.generic_string() + "\"";
    }
    sru::util::cmd(command);
    if (std::filesystem::exists(retard_cache)) {
        std::filesystem::remove(abs_pdf_file);
        std::filesystem::rename(retard_cache, abs_pdf_file);
    }
    return true;
}

auto decrease_size(const std::filesystem::path& path, size_t from, size_t size) -> bool {
    const auto abs_pdf_file = std::filesystem::absolute(path.lexically_normal());
    std::string command{};
    command = qpdf_settings.get_bin() + " \"" + abs_pdf_file.generic_string() + "\" --replace-input --stream-data=preserve --pages . 1-" +
              std::to_string(size) + " -- ";

    sru::util::cmd(command);
    return std::filesystem::exists(abs_pdf_file);
}

auto empty_cache() -> void { std::filesystem::remove_all(qpdf_settings.get_cache_path()); }
} // namespace sru::qpdf
