#pragma once
#include "../util/re_accel.h"
#include "../util/util.h"
#include "page_config.h"
#include "string_obj.h"
#include <chrono>
#include <ctll.hpp>
#include <ctre.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <rapidjson/document.h>
#include <string>
#include <utility>
#include <vector>

namespace sru::pdf {
static float pagecounter = 0;
class PdfPage {
    std::string raw;
    std::vector<sru::pdf::StringObject> objs;
    const PageConfig config;

  public:
    PdfPage(std::string raw, const PageConfig config)
        : raw{raw}, config{config} {
        const auto start = std::chrono::steady_clock::now();
        if (const auto found = sru::util::regex_accel[config.obj_regex](raw)) {
            auto color = sru::util::Color{0, 0, 0};
            for (const auto &x : found.value()) {

                if (x.at(1) == "") {
                    objs.emplace_back(
                        color, sru::util::svoi(x.at(4)),
                        sru::util::svof(x.at(5)), std::string{x.at(6)},
                        sru::util::Cordinate{sru::util::svof(x.at(7)),
                                             sru::util::svof(x.at(8))},
                        std::string{x.at(9)});
                } else {
                    color.r = sru::util::svof(x.at(1));
                    color.g = sru::util::svof(x.at(2));
                    color.b = sru::util::svof(x.at(3));
                }
            }
            std::cout << "Amount of objects: " << objs.size() << "\n";
        }
        const auto end = std::chrono::steady_clock::now();
        std::cout << "Pdf page creation (sec) = "
                  << (std::chrono::duration_cast<std::chrono::microseconds>(
                          end - start)
                          .count()) /
                         1000000.0
                  << "\n";
    }
    const std::vector<sru::pdf::StringObject> &getObjects() const;
};

}; // namespace sru::pdf
