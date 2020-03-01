#include "config.h"
#include "src/pdf/anchor_config.h"
#include "src/pdf/calc_config.h"
#include "src/pdf/object_config.h"
#include "src/pdf/page_config.h"
#include "src/pdf/pdf_cluster.h"
#include "src/util/util.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <rapidjson/schema.h>
#include <string_view>
#include <utility>
#include <vector>

auto main(int argc, char** argv) -> int {
    const auto start = std::chrono::steady_clock::now();
    [[maybe_unused]] int program_ver = 1;
    [[maybe_unused]] int program_min_ver = 0;
    [[maybe_unused]] int program_patch_ver = 0;
#ifdef DEV_BUILT
    std::cout << "SRU Dev build" << std::endl;
#else
#ifdef NDEBUG
    std::cout << "SRU Release ";
#else
    std::cout << "SRU Debug ";
#endif
    std::cout << "v" << program_ver << "." << program_min_ver << "." << program_patch_ver << std::endl;
#endif
    std::cout << "Working dir: " << std::filesystem::current_path().generic_string() << std::endl;

    auto json_config = sru::util::QFileRead("./reference.json");
    if (!json_config) {
        std::cout << "Config file not found.\n" << std::endl;
        return -1;
    }

    rapidjson::Document d;
    rapidjson::ParseResult check = d.ParseInsitu(json_config.value().data());
    if (!check) {
        std::cout << "Invalid JSON.\n" << std::endl;
        return -1;
    }

    rapidjson::Document sd;
    sd.Parse(SRU_SCHEMA);
    rapidjson::SchemaDocument schema{sd};
    rapidjson::SchemaValidator validator{schema};
    if (!d.Accept(validator)) {
        std::cout << "JSON does not follow schema.\n" << std::endl;
        return -1;
    }

    std::cout << "JSON successfully parsed.\n";

    sru::pdf::DATE_PROVIDER = d["date_provider"].GetInt();

    for (auto& obb : d["pages"].GetArray()) {
        std::vector<size_t> groups;
        for (auto& val : obb["groups"].GetArray()) {
            groups.push_back(val.Get<size_t>());
        }
        sru::pdf::PageConfig page{obb["page_name"].GetString(), obb["priority"].GetInt(),     obb["mutate_in_final"].GetString(),
                                  obb["regex_id"].GetString(),  obb["obj_regex"].GetString(), groups};
        sru::pdf::PageConfigPool.push_back(std::move(page));
    }
    for (auto& obb : d["anchors"].GetArray()) {
        std::vector<int> sub_groups;
        for (auto& val : obb["sub_groups"].GetArray()) {
            sub_groups.push_back(val.Get<int>());
        }
        sru::pdf::AnchorConfig anchor{obb["id"].Get<size_t>(),
                                      obb.HasMember("is_virtual") ? obb["is_virtual"].GetBool() : false,
                                      sru::util::Coordinate(obb["x"].GetFloat(), obb["y"].GetFloat()),
                                      obb["anchor_name"].GetString(),
                                      obb["content_id"].GetString(),
                                      obb.HasMember("content_") ? obb["content_"].GetString() : "",
                                      obb.HasMember("content_alt") ? obb["content_alt"].GetString() : "",
                                      obb.HasMember("save_anchor") ? obb["save_anchor"].GetBool() : true,
                                      sub_groups};
        sru::pdf::AnchorConfigPool.push_back(std::move(anchor)); // REMINDER TO DO THIS FOR EVERYTHING
    }
    for (auto& obb : d["objects"].GetArray()) {

        std::vector<size_t> calcs;
        if (obb.HasMember("calcs")) {
            for (auto& val : obb["calcs"].GetArray()) {
                calcs.push_back(val.Get<int>());
            }
        }

        sru::pdf::ObjectConfig config{obb["id"].Get<size_t>(),
                                      obb["object_name"].GetString(),
                                      obb.HasMember("margin_x") ? obb["margin_x"].GetFloat() : 0,
                                      obb.HasMember("margin_y") ? obb["margin_y"].GetFloat() : 0,
                                      obb.HasMember("anchor_margin_x") ? obb["anchor_margin_x"].GetInt() : -1,
                                      obb.HasMember("anchor_margin_y") ? obb["anchor_margin_y"].GetInt() : -1,
                                      obb.HasMember("object_count") ? obb["object_count"].Get<size_t>() : 1,
                                      obb.HasMember("y_object_spacing") ? obb["y_object_spacing"].GetFloat() : 0,
                                      obb.HasMember("text_justify") ? obb["text_justify"].GetFloat() : 0,
                                      obb.HasMember("sticky_id") ? obb["sticky_id"].GetInt() : -1,
                                      calcs};
        sru::pdf::ObjectConfigPool.push_back(std::move(config));
    }
    for (auto& obb : d["calcs"].GetArray()) {
        std::vector<bool> sort_settings;
        if (obb.HasMember("sort_settings")) {
            for (auto& val : obb["sort_settings"].GetArray()) {
                sort_settings.push_back(val.Get<bool>());
            }
        }

        std::vector<int> re_extract_order;
        if (obb.HasMember("re_extract_order")) {
            for (auto& val : obb["re_extract_order"].GetArray()) {
                re_extract_order.push_back(val.Get<int>());
            }
        }
        std::array<size_t, 2> avrg_source_id{0, 0};
        std::array<size_t, 2> avrg_base_id{0, 0};

        if (obb.HasMember("avrg_source_id")) {
            avrg_source_id[0] = obb["avrg_source_id"].GetArray()[0].Get<size_t>();
            avrg_source_id[1] = obb["avrg_source_id"].GetArray()[1].Get<size_t>();
            avrg_base_id[0] = obb["avrg_base_id"].GetArray()[0].Get<size_t>();
            avrg_base_id[1] = obb["avrg_base_id"].GetArray()[1].Get<size_t>();
        }

        sru::pdf::CalcConfig config{obb["id"].Get<size_t>(),
                                    obb["calc_name"].GetString(),
                                    obb.HasMember("maximum_values") ? obb["maximum_values"].Get<size_t>() : 0,
                                    obb.HasMember("round_cut_off") ? obb["round_cut_off"].GetFloat() : 0,
                                    obb.HasMember("decimal_points") ? obb["decimal_points"].GetInt() : 0,
                                    obb.HasMember("calc_mode") ? obb["calc_mode"].GetString() : "",
                                    obb.HasMember("avrg_self") ? obb["avrg_self"].GetBool() : false,
                                    avrg_source_id,
                                    avrg_base_id,
                                    obb.HasMember("avrg_multiplier") ? obb["avrg_multiplier"].GetFloat() : -1,
                                    obb.HasMember("overflow_threshold") ? obb["overflow_threshold"].GetFloat() : -1,
                                    obb.HasMember("zfill") ? obb["zfill"].GetInt() : 0,
                                    sort_settings,
                                    re_extract_order,
                                    obb.HasMember("regex") ? obb["regex"].GetString() : ""};
        sru::pdf::CalcConfigPool.push_back(std::move(config));
    }
    std::vector<std::filesystem::path> pdf_file_paths{};
    std::filesystem::path import_path{d["import_path"].GetString()};
    if (!std::filesystem::exists(import_path)) {
        std::filesystem::create_directory(import_path);
        std::cout << "Import folder not found, created directory." << std::endl;
        return -1;
    }
    for (auto& file : std::filesystem::directory_iterator{import_path}) {
        if (const auto& file_path = file.path(); boost::iequals(file_path.extension().generic_string(), ".pdf")) {
            pdf_file_paths.push_back(file_path);
        }
    }

    auto cache_path = import_path;
    cache_path.append("cache");
    sru::qpdf::set_cache_path(cache_path);
    sru::pdf::base_path = std::filesystem::path{d["base_path"].GetString()};
    if (!std::filesystem::exists(sru::pdf::base_path) && !sru::pdf::base_path.empty()) {
        std::cout << "Provided base pdf doesnt exist." << std::endl;
        return -1;
    }
    sru::pdf::base_size = d["base_size"].GetInt();
    sru::pdf::graph_path = std::filesystem::path{d["graph_path"].GetString()};
    if (!std::filesystem::exists(sru::pdf::base_path)) {
        std::cout << "Provided graph doesnt exist." << std::endl;
        return -1;
    }
    auto cluster = sru::pdf::PdfCluster{pdf_file_paths};
    cluster.export_merged();
    sru::qpdf::empty_cache();

    const auto end = std::chrono::steady_clock::now();

    std::cout << "Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000000.0 << std::endl;
    return 0;
}
