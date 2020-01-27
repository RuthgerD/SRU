#include "config.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "src/pdf_structures/pdf_cluster.h"
#include "src/pdf_structures/string_obj.h"
#include "src/util/qpdf_binding.h"
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
#include <string>
#include <string_view>
#include <utility>
#include <vector>

int main(int argc, char **argv) {

    const auto start = std::chrono::steady_clock::now();

    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "Help screen")(
        "config", po::value<std::string>(), "file path");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    std::optional<std::string> json_config{};
    auto qpdf = sru::util::Qpdf{"import/cache"};

    json_config = sru::util::QFileRead("/home/ruthger/config.json");
    if (!json_config) {
        std::cout << "Files not found." << std::endl;
        return 1;
    }
    // if (vm.count("config")) {
    // } else {
    //     return 1;
    // }

    std::vector<std::filesystem::path> pdf_file_paths{};
    for (auto &file : std::filesystem::directory_iterator{"./import"}) {
        if (auto file_path = file.path(); file_path.extension() == ".pdf") {
            pdf_file_paths.push_back(file_path);
        }
    }
    auto cluster =
        sru::pdf::PdfCluster{pdf_file_paths, qpdf, json_config.value()};

    const auto &test = cluster.getConfig()["group_configs"]["End"]["time"];
    for (auto &v : test.GetObject()) {
        auto &thing = test[v.name.GetString()];
        if (v.value.IsInt())
            std::cout << test[v.name.GetString()].GetInt() << "\n";
    }

    // std::for_each(objs.begin(), objs.end(), [](sru::pdf::StringObject ob) {
    //     std::cout << ob.toString();
    // });
    const auto end = std::chrono::steady_clock::now();
    std::cout << "Time difference (sec) = "
              << (std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                        start)
                      .count()) /
                     1000000.0
              << std::endl;
    return 0;
}
