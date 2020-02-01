#include "config.h"
#include "src/pdf_structures/pdf_cluster.h"
#include "src/pdf_structures/object_config.h"
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

    json_config = sru::util::QFileRead("/home/ruthgerd/config.json");
    if (!json_config) {
        std::cout << "Files not found." << std::endl;
        return 1;
    }
    // if (vm.count("config")) {
    // } else {
    //     return 1;
    // }

    std::vector<std::filesystem::path> pdf_file_paths{};
    // On unix based systems the binary isnt stored in a place where the user
    // has easy access to, add a --in option or something
    for (auto &file : std::filesystem::directory_iterator{"./import"}) {
        std::cout << file.path().lexically_normal().generic_string() << "/n";
        if (auto file_path = file.path(); file_path.extension() == ".pdf") {
            pdf_file_paths.push_back(file_path);
        }
    }
    auto cluster =
        sru::pdf::PdfCluster{pdf_file_paths, qpdf};

    const auto end = std::chrono::steady_clock::now();
    std::cout << "Time difference (sec) = "
              << (std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                        start)
                      .count()) /
                     1000000.0
              << std::endl;
    return 0;
}
