#include "pdf_page.h"
#include "../util/util.h"
#include "anchor_config.h"
#include "object_config.h"
#include <cmath>
#include <utility>
#include <vector>

namespace sru::pdf {
PdfPage::PdfPage(std::string raw, const PageConfig config) : raw{std::move(raw)}, config{config} {}
void PdfPage::indexObjects() {
    objs.clear();
    marked_objs.clear();
    if (const auto found = sru::util::regex_accel[config.obj_regex](raw); found) {
        auto color = sru::util::Color{0, 0, 0};
        for (const auto& x : *found) {

            if (x.at(1) == "") {
                objs.emplace_back(color, sru::util::svto<int>(x.at(4)), sru::util::svto<float>(x.at(5)), std::string{x.at(6)},
                                  sru::util::Coordinate{sru::util::svto<float>(x.at(7)), sru::util::svto<float>(x.at(8))}, std::string{x.at(9)});
            } else {
                color.r = sru::util::svto<float>(x.at(1));
                color.g = sru::util::svto<float>(x.at(2));
                color.b = sru::util::svto<float>(x.at(3));
            }
        }
        // std::cout << "Amount of objects: " << objs.size() << std::endl;
    }
    for (auto& obj : objs) {
        for (auto anchor_conf_id : config.groups) {
            if (const auto anchor_conf = getAnchorConfig(anchor_conf_id); anchor_conf) {
                if (sru::util::re_match(anchor_conf->content_id, obj.getContent())) {
                    if (anchor_conf->save_anchor) {
                        anchor_objs.emplace(anchor_conf_id, obj);
                    }
                }
            }
        }
    }
    for (const auto anchor_pair : anchor_objs) {
        if (const auto anchor_conf = getAnchorConfig(anchor_pair.first); anchor_conf) {
            const auto& anchor_obj = anchor_pair.second;
            for (auto object_conf_id : anchor_conf.value().sub_groups) {
                if (const auto object_conf_opt = getObjectConfig(object_conf_id); object_conf_opt) {
                    const auto& object_conf = object_conf_opt.value();

                    const float& ref_x = anchor_obj.get().getPosition().getX();
                    const float& ref_y = anchor_obj.get().getPosition().getY();

                    const float max_x = ref_x + object_conf.margin_x;
                    const float max_y = ref_y + object_conf.margin_y;

                    const int object_count = object_conf.object_count;
                    int sticky_id = object_conf.sticky_id;

                    int count_start = 0;
                    int found_count = 0;
                    int captured_count = 0;

                    for (auto& comp_obj : objs) {
                        const float comp_x = std::fabs(comp_obj.getPosition().getX());
                        const float comp_y = std::fabs(comp_obj.getPosition().getY());

                        if ((comp_x < max_x && comp_x > ref_x && comp_y >= max_y && comp_y <= ref_y) ||
                            (comp_x <= max_x && comp_x >= ref_x && comp_y > max_y && comp_y < ref_y)) {
                            if (captured_count == object_count) {
                                break;
                            }
                            if (found_count >= count_start && &comp_obj != &anchor_obj.get()) {
                                if (sticky_id < 0) {
                                    if (marked_objs.find(object_conf_id) == marked_objs.end()) {

                                        marked_objs.emplace(object_conf_id, std::vector<std::reference_wrapper<sru::pdf::StringObject>>{});
                                    }
                                    marked_objs.at(object_conf_id).emplace_back(comp_obj);
                                } else {
                                    if (stickied_objs.find(sticky_id) == stickied_objs.end() && sticky_id >= 0) {
                                        stickied_objs.emplace(sticky_id, std::vector<std::reference_wrapper<sru::pdf::StringObject>>{});
                                    }
                                    marked_objs.at(sticky_id).emplace_back(comp_obj);
                                }
                                ++captured_count;
                            }
                            ++found_count;
                        }
                    }
                    count_start += object_count;
                }
            }
        }
    }
}
void PdfPage::printObjects() const {
    for (auto anchor_pair : anchor_objs) {
        if (auto conf = getAnchorConfig(anchor_pair.first)) {
            std::cout << conf.value().name << ":\n";
            for (auto id : conf.value().sub_groups) {
                for (auto& obj : marked_objs.at(id)) {
                    std::cout << "* " << obj.get().getContent() << "\n";
                }
            }
        }
    }
}
const std::vector<sru::pdf::StringObject>& PdfPage::getObjects() const { return objs; }
std::vector<std::reference_wrapper<sru::pdf::StringObject>> PdfPage::getMarkedObjects(int id) const {
    if (marked_objs.find(id) != marked_objs.end()) {
        return marked_objs.at(id);
    } else {
        return std::vector<std::reference_wrapper<sru::pdf::StringObject>>{}; // maybe use optional instead
    }
}
} // namespace sru::pdf