#include "src/pdf_structures/string_obj.h"
#include "src/util/util.h"
#include <algorithm>
#include <boost/program_options.hpp>
#include <chrono>
#include <cstdio>
#include <ctll.hpp>
#include <ctre.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

static constexpr auto pattern = ctll::fixed_string{
    R"(BT\n/F([0-9]+?) ([0-9.]+?) Tf\n([0-9. ]+) ([0-9.]+?) ([0-9.]+?) Tm\n\((.+?)\)Tj\nET\n)"};

int main(int argc, char **argv) {
    const auto start = std::chrono::steady_clock::now();

    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "Help screen")(
        "file", po::value<std::string>(), "file path");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    std::optional<std::string> pdfdata{};
    if (vm.count("file")) {
        pdfdata = sru::util::QFileRead(vm["file"].as<std::string>());
        if (!pdfdata) {
            std::cout << "File not found." << std::endl;
            return 1;
        }
    } else {
        return 1;
    }

    auto found = ctre::range<pattern>(pdfdata.value());

    auto objs = std::vector<sru::pdf::StringObject>{};
    for (auto x : found) {
        objs.push_back(sru::pdf::StringObject{
            sru::util::Color{0, 0, 0}, std::stoi(x.get<1>().to_string()),
            std::stod(x.get<2>().to_string()), x.get<3>().to_string(),
            sru::util::Cordinate{std::stod(x.get<4>().to_string()),
                                 std::stod(x.get<5>().to_string())},
            x.get<6>().to_string()});
    }

    const auto end = std::chrono::steady_clock::now();

    std::for_each(objs.begin(), objs.end(), [](sru::pdf::StringObject ob) {
        std::cout << ob.toString();
    });
    std::cout << "Time difference (sec) = "
              << (std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                        start)
                      .count()) /
                     1000000.0
              << std::endl;
    return 0;
}
