#include "config.h"
#include "src/pdf_structures/anchor_config.h"
#include "src/pdf_structures/object_config.h"
#include "src/pdf_structures/page_config.h"
#include "src/pdf_structures/pdf_cluster.h"
#include "src/util/util.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/program_options.hpp>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <rapidjson/schema.h>
#include <string_view>
#include <utility>
#include <vector>

auto main(int argc, char** argv) -> int {
    std::cout << std::filesystem::current_path().c_str() << std::endl;
    // std::ios::sync_with_stdio(false);
    namespace p_opt = boost::program_options;
    p_opt::options_description desc("Allowed options");
    desc.add_options()("help,h", "Help screen")("config", p_opt::value<std::string>(), "file path");
    p_opt::variables_map vm;
    p_opt::store(p_opt::parse_command_line(argc, argv, desc), vm);
    p_opt::notify(vm);

    // if (vm.count("config")) {
    // } else {
    //     return 1;
    // }

    // JSON schema parsing
    const auto start = std::chrono::steady_clock::now();
    auto json_config = sru::util::QFileRead("./reference.json");
    if (!json_config) {
        std::cout << "Files not found.\n" << std::endl;
        return 1;
    }

    rapidjson::Document d;
    rapidjson::ParseResult check = d.ParseInsitu(json_config.value().data());
    if (!check) {
        std::cout << "Invalid JSON.\n" << std::endl;
        return 1;
    }

    rapidjson::Document sd;
    sd.Parse(SRU_SCHEMA);
    rapidjson::SchemaDocument schema{sd};
    rapidjson::SchemaValidator validator{schema};
    if (!d.Accept(validator)) {
        std::cout << "JSON does not follow schema.\n" << std::endl;
        return 1;
    }

    std::cout << "JSON successfully parsed.\n";

    for (auto& obb : d["pages"].GetArray()) {
        std::vector<int> groups;
        for (auto& val : obb["groups"].GetArray()) {
            groups.push_back(val.Get<int>());
        }
        sru::pdf::PageConfig page{obb["regex_id"].GetString(), obb["obj_regex"].GetString(), groups};
        sru::pdf::PageConfigPool.push_back(std::move(page));
    }
    for (auto& obb : d["anchors"].GetArray()) {
        std::vector<int> sub_groups;
        for (auto& val : obb["sub_groups"].GetArray()) {
            sub_groups.push_back(val.Get<int>());
        }
        sru::pdf::AnchorConfig anchor{obb["id"].GetInt(),
                                      obb["anchor_name"].GetString(),
                                      obb["content_id"].GetString(),
                                      obb["content_"].GetString(),
                                      obb["content_alt"].GetString(),
                                      obb["save_anchor"].GetBool(),
                                      sub_groups};
        sru::pdf::AnchorConfigPool.push_back(std::move(anchor)); // REMINDER TO DO THIS FOR EVERYTHING
    }
    for (auto& obb : d["objects"].GetArray()) {
        std::vector<std::string> calc_modes;
        for (auto& val : obb["calc_modes"].GetArray()) {
            calc_modes.push_back(val.Get<std::string>());
        }

        std::vector<bool> sort_settings;
        for (auto& val : obb["sort_settings"].GetArray()) {
            sort_settings.push_back(val.Get<bool>());
        }

        std::vector<std::string> regexs;
        for (auto& val : obb["regexs"].GetArray()) {
            regexs.push_back(val.Get<std::string>());
        }

        std::vector<int> re_extract_order;
        for (auto& val : obb["re_extract_order"].GetArray()) {
            re_extract_order.push_back(val.Get<int>());
        }
        sru::pdf::ObjectConfig testing{obb["id"].GetInt(),
                                       obb["object_name"].GetString(),
                                       obb["text_justify"].GetFloat(),
                                       obb["maximum_values"].GetInt(),
                                       obb["y_object_spacing"].GetFloat(),
                                       obb["round_cut_off"].GetFloat(),
                                       obb["decimal_points"].GetInt(),
                                       calc_modes,
                                       obb["avrg_self"].GetBool(),
                                       obb["avrg_source_id"].GetInt(),
                                       obb["avrg_base_id"].GetInt(),
                                       obb["avrg_multiplier"].GetInt(),
                                       obb["overflow_threshold"].GetFloat(),
                                       sort_settings,
                                       re_extract_order,
                                       regexs,
                                       obb["margin_x"].GetFloat(),
                                       obb["margin_y"].GetFloat(),
                                       obb["object_count"].GetInt(),
                                       obb["sticky_id"].GetInt()};
        sru::pdf::ObjectConfigPool.push_back(std::move(testing));
    }
    std::vector<std::filesystem::path> pdf_file_paths{};
    // On unix based systems the binary isnt stored in a place where the user
    // has easy access to, add a --in option or something
    for (auto& file : std::filesystem::directory_iterator{"./import"}) {
        if (const auto& file_path = file.path(); boost::iequals(file_path.extension().generic_string(), ".pdf")) {
            pdf_file_paths.push_back(file_path);
        }
    }
    auto qpdf = sru::util::Qpdf{"import/cache"};
    auto cluster = sru::pdf::PdfCluster{pdf_file_paths, qpdf};

    const auto end = std::chrono::steady_clock::now();

    std::cout << "Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000000.0 << std::endl;
    return 0;
}
