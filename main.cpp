#include "config.h"
#include "src/pdf_structures/anchor_config.h"
#include "src/pdf_structures/object_config.h"
#include "src/pdf_structures/page_config.h"
#include "src/pdf_structures/pdf_cluster.h"
#include "src/util/util.h"
#include <algorithm>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctll.hpp>
#include <ctre.hpp>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <rapidjson/schema.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

int main(int argc, char **argv) {

    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "Help screen")(
        "config", po::value<std::string>(), "file path");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

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

    std::cout << "JSON succesfully parsed.\n";

    for (auto &obb : d["pages"].GetArray()) {
        std::vector<int> groups;
        for (auto &val : obb["groups"].GetArray()) {
            groups.push_back(val.Get<int>());
        }
        const sru::pdf::PageConfig page{obb["regex_id"].GetString(),
                                        obb["obj_regex"].GetString(), groups};
        sru::pdf::PageConfigPool.push_back(page);
    }
    for (auto &obb : d["anchors"].GetArray()) {
        std::vector<int> sub_groups;
        for (auto &val : obb["sub_groups"].GetArray()) {
            sub_groups.push_back(val.Get<int>());
        }
        const sru::pdf::AnchorConfig anchor{
            obb["id"].GetInt(),           obb["content_id"].GetString(),
            obb["content_"].GetString(),  obb["content_alt"].GetString(),
            obb["save_anchor"].GetBool(), sub_groups};
        sru::pdf::AnchorConfigPool.push_back(anchor);
    }
    for (auto &obb : d["objects"].GetArray()) {
        std::vector<std::string> calc_modes;
        for (auto &val : obb["calc_modes"].GetArray()) {
            calc_modes.push_back(val.Get<std::string>());
        }

        std::vector<bool> sort_settings;
        for (auto &val : obb["sort_settings"].GetArray()) {
            sort_settings.push_back(val.Get<bool>());
        }

        std::vector<std::string> regexs;
        for (auto &val : obb["regexs"].GetArray()) {
            regexs.push_back(val.Get<std::string>());
        }

        const sru::pdf::ObjectConfig testing{
            obb["id"].GetInt(),
            obb["text_justify"].GetFloat(),
            obb["maximum_values"].GetInt(),
            obb["y_object_spacing"].GetFloat(),
            obb["round_cut_off"].GetFloat(),
            obb["is_int"].GetBool(),
            obb["minimum_value"].GetFloat(),
            calc_modes,
            obb["avrg_self"].GetBool(),
            obb["avrg_source_group"].GetString(),
            obb["avrg_source_sub_group"].GetString(),
            obb["avrg_base_group"].GetString(),
            obb["avrg_base_sub_group"].GetString(),
            obb["avrg_multiplier"].GetInt(),
            obb["overflow_threshold"].GetFloat(),
            sort_settings,
            obb["re_comp_name"].GetString(),
            obb["re_group_count"].GetInt(),
            regexs,
            obb["margin_x"].GetFloat(),
            obb["margin_y"].GetFloat(),
            obb["object_count"].GetInt()};
        sru::pdf::ObjectConfigPool.push_back(testing);
    }
    std::vector<std::filesystem::path> pdf_file_paths{};
    // On unix based systems the binary isnt stored in a place where the user
    // has easy access to, add a --in option or something
    for (auto &file : std::filesystem::directory_iterator{"./import"}) {
        if (auto file_path = file.path(); file_path.extension() == ".pdf") {
            pdf_file_paths.push_back(file_path);
        }
    }
    auto qpdf = sru::util::Qpdf{"import/cache"};
    auto cluster = sru::pdf::PdfCluster{pdf_file_paths, qpdf};
    const auto end = std::chrono::steady_clock::now();

    std::cout << "Time difference (sec) = "
              << (std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                        start)
                      .count()) /
                     1000000.0
              << std::endl;
    return 0;
}
