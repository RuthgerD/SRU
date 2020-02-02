#include "pdf_page.h"
#include "../util/util.h"
#include "anchor_config.h"
#include "object_config.h"
#include <math.h>
#include <vector>

namespace sru::pdf {
void PdfPage::indexObjects() {
    objs.clear();
    marked_objs.clear();
    if (const auto found = sru::util::regex_accel[config.obj_regex](raw)) {
        auto color = sru::util::Color{0, 0, 0};
        for (const auto &x : found.value()) {

            if (x.at(1) == "") {
                objs.emplace_back(
                    color, sru::util::svoi(x.at(4)), sru::util::svof(x.at(5)),
                    std::string{x.at(6)},
                    sru::util::Cordinate{sru::util::svof(x.at(7)),
                                         sru::util::svof(x.at(8))},
                    std::string{x.at(9)});
            } else {
                color.r = sru::util::svof(x.at(1));
                color.g = sru::util::svof(x.at(2));
                color.b = sru::util::svof(x.at(3));
            }
        }
        // std::cout << "Amount of objects: " << objs.size() << std::endl;
    }
    for (auto &obj : objs) {
        for (auto anchor_conf_id : config.groups) {
            if (const auto anchor_conf_opt = getAnchorConfig(anchor_conf_id)) {
                const auto &anchor_conf = anchor_conf_opt.value();
                if (sru::util::re_search(anchor_conf.content_id,
                                         obj.getContent())) {
                    if (anchor_conf.save_anchor) {
                        anchor_objs.emplace(anchor_conf_id, &obj);
                    }
                }
            }
        }
    }
    for (const auto anchor_pair : anchor_objs) {
        // std::cout << "Anchor: " << anchor_pair.second->getContent() << "\n";
        if (const auto anchor_conf = getAnchorConfig(anchor_pair.first)) {
            const auto anchor_obj = anchor_pair.second;
            for (auto object_conf_id : anchor_conf.value().sub_groups) {
                if (const auto object_conf_opt =
                        getObjectConfig(object_conf_id)) {
                    const auto &object_conf = object_conf_opt.value();

                    const float &ref_x = anchor_obj->getPosition().getX();
                    const float &ref_y = anchor_obj->getPosition().getY();

                    const float max_x = ref_x + object_conf.margin_x;
                    const float max_y = ref_y + object_conf.margin_y;

                    float count_start = 0;
                    const float object_count = object_conf.object_count;
                    float found_count = 0;
                    float captured_count = 0;

                    // TODO: Support stickies.
                    for (auto &comp_obj : objs) {
                        const float &comp_x =
                            std::fabs(comp_obj.getPosition().getX());
                        const float &comp_y =
                            std::fabs(comp_obj.getPosition().getY());

                        if (comp_x < max_x && comp_x > ref_x &&
                                comp_y >= max_y && comp_y <= ref_y ||
                            comp_x <= max_x && comp_x >= ref_x &&
                                comp_y > max_y && comp_y < ref_y) {

                            if (captured_count != object_count) {
                                if (found_count >= count_start &&
                                    &comp_obj != anchor_obj) {
                                    if (marked_objs.find(object_conf_id) ==
                                        marked_objs.end()) {
                                        marked_objs.emplace(
                                            object_conf_id,
                                            std::vector<StringObject *>{});
                                    }
                                    // std::cout
                                    //     << "obj:" << comp_obj.getContent()
                                    //     << " ref_x: " << ref_x
                                    //     << " ref_y: " << ref_y
                                    //     << " comp_x: " << comp_x
                                    //     << " comp_y: " << comp_y
                                    //     << " margin_x: " <<
                                    //     object_conf.margin_x
                                    //     << " margin_y: " <<
                                    //     object_conf.margin_y
                                    //     << "\n";
                                    marked_objs.at(object_conf_id)
                                        .emplace_back(&comp_obj);
                                    ++captured_count;
                                }
                                ++found_count;
                            } else {
                                break;
                            }
                        }
                    }
                    count_start += object_count;
                }
            }
        }
    }
    printObjects();
}
void PdfPage::printObjects() {
    // for (auto pair : marked_objs) {
    //     for (auto obj : pair.second) {
    //         std::cout << obj->getContent() << "\n";
    //     }
    // }
    for (auto anchor_pair : anchor_objs) {
        if (auto conf = getAnchorConfig(anchor_pair.first)) {
            std::cout << conf.value().name << ":\n";
            for (auto id : conf.value().sub_groups) {
                for (auto obj : marked_objs.at(id)) {
                    std::cout << "* " << obj->getContent() << "\n";
                }
            }
        }
    }
}
const std::vector<sru::pdf::StringObject> &PdfPage::getObjects() const {
    return objs;
}
} // namespace sru::pdf