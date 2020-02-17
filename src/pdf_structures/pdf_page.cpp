#include "pdf_page.h"
#include "../util/util.h"
#include "anchor_config.h"
#include "object_config.h"
#include <cmath>
#include <utility>
#include <vector>

namespace sru::pdf {
PdfPage::PdfPage(std::string raw, const PageConfig& config) : raw{std::move(raw)}, config{config} {}
auto PdfPage::getConfig() const -> PageConfig { return config; }
void PdfPage::indexObjects() {
    objs.clear();
    marked_objs.clear();
    stickied_objs.clear();
    anchor_objs.clear();
    if (const auto found = sru::util::re_search(config.obj_regex, raw); found) {
        auto color = sru::util::Color{0, 0, 0};
        for (const auto& x : *found) {

            if (x[1] == "") {
                objs.emplace_back(color, sru::util::svto<int>(x[4]), sru::util::svto<float>(x[5]), std::string{x[6]},
                                  sru::util::Coordinate{sru::util::svto<float>(x[7]), sru::util::svto<float>(x[8])}, std::string{x[9]});
            } else {
                color.r = sru::util::svto<float>(x[1]);
                color.g = sru::util::svto<float>(x[2]);
                color.b = sru::util::svto<float>(x[3]);
            }
        }
    }
    for (auto& obj : objs) {
        for (auto anchor_conf_id : config.groups) {
            if (const auto anchor_conf_opt = getAnchorConfig(anchor_conf_id); anchor_conf_opt) {
                const auto& anchor_conf = *anchor_conf_opt;
                auto conf_x = anchor_conf.position.getX();
                auto conf_y = anchor_conf.position.getY();
                if (anchor_conf.is_virtual || (std::fabs(conf_y) / conf_y) * obj.getPosition().getY() >= conf_y ||
                    (std::fabs(conf_x) / conf_x) * obj.getPosition().getX() <= conf_x ||
                    !sru::util::re_match(anchor_conf.content_id, obj.getContent())) {
                    continue;
                }
                if (anchor_conf.save_anchor) {
                    anchor_objs.emplace(anchor_conf_id, &obj - objs.data());
                }
                anchor_positions.emplace(anchor_conf_id, obj.getPosition());
            }
        }
    }
    for (const auto& anchor_conf_id : config.groups) {
        if (const auto anchor_conf = getAnchorConfig(anchor_conf_id); anchor_conf) {
            if (anchor_conf->is_virtual) {
                anchor_positions.emplace(anchor_conf_id, anchor_conf->position);
            }
        }
    }
    for (const auto anchor_pair : anchor_positions) {
        if (const auto anchor_conf = getAnchorConfig(anchor_pair.first); anchor_conf) {
            const auto& anchor_obj = anchor_pair.second;
            int count_start = 0;
            for (auto object_conf_id : anchor_conf->sub_groups) {
                if (const auto object_conf_opt = getObjectConfig(object_conf_id); object_conf_opt) {
                    const auto& object_conf = object_conf_opt.value();

                    const float& ref_x = anchor_obj.getX();
                    const float& ref_y = anchor_obj.getY();

                    const float max_x = ref_x + object_conf.margin_x;
                    const float max_y = ref_y + object_conf.margin_y;

                    const int object_count = object_conf.object_count;
                    int sticky_id = object_conf.sticky_id;

                    int found_count = 0;
                    int captured_count = 0;

                    for (auto& comp_obj : objs) {
                        const float comp_x = std::fabs(comp_obj.getPosition().getX());
                        const float comp_y = std::fabs(comp_obj.getPosition().getY());

                        // TODO: use sru::util::Coordinate when its implemented
                        if ((comp_x < max_x && comp_x > ref_x && comp_y >= max_y && comp_y <= ref_y) ||
                            (comp_x <= max_x && comp_x >= ref_x && comp_y > max_y && comp_y < ref_y) ||
                            (comp_x == ref_x && comp_y == ref_y && !anchor_conf->save_anchor)) {
                            if (captured_count == object_count) {
                                break;
                            }
                            if (found_count >= count_start) {
                                if (sticky_id < 0) {
                                    if (marked_objs.find(object_conf_id) == marked_objs.end()) {

                                        marked_objs.emplace(object_conf_id, std::vector<int>{});
                                    }
                                    marked_objs.at(object_conf_id).emplace_back(&comp_obj - objs.data());
                                } else {
                                    if (stickied_objs.find(sticky_id) == stickied_objs.end() && sticky_id >= 0) {
                                        stickied_objs.emplace(sticky_id, std::vector<int>{});
                                    }
                                    marked_objs.at(sticky_id).emplace_back(&comp_obj - objs.data());
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
    if (anchor_positions.empty()) {
        std::cout << "page: no objects to display" << std::endl;
    }
    for (auto anchor_pair : anchor_positions) {
        if (auto conf = getAnchorConfig(anchor_pair.first)) {
            std::cout << conf.value().name << ":\n";
            for (auto id : conf.value().sub_groups) {
                if (marked_objs.find(id) != marked_objs.end()) {
                    for (auto& obj : marked_objs.at(id)) {
                        std::cout << "* " << objs[obj].getContent() << "\n";
                    }
                }
            }
        }
    }
}
auto PdfPage::getObjects() const -> const std::vector<sru::pdf::StringObject>& { return objs; }
auto PdfPage::getObjects() -> std::vector<sru::pdf::StringObject>& { return objs; }
auto PdfPage::getRaw() const -> const std::string& { return raw; }
auto PdfPage::getMarkedObjects(int id) -> std::vector<std::reference_wrapper<StringObject>> {
    std::vector<std::reference_wrapper<StringObject>> wrapped{};
    if (marked_objs.find(id) != marked_objs.end()) {
        for (const int& x : marked_objs.at(id)) {
            wrapped.emplace_back(objs[x]);
        }
    }
    return wrapped;
}

auto PdfPage::db_getObjects() -> const std::vector<StringObject>& { return objs; }
auto PdfPage::db_getMarkedObjects(int id) -> std::vector<int> {
    if (marked_objs.find(id) != marked_objs.end()) {
        return marked_objs[id];
    }
    return {};
}
auto PdfPage::db_updateObject(int id, StringObject obj) -> bool {
    if (id >= objs.size()) {
        return false;
    }
    update_staging.erase(id);
    update_staging.emplace(id, std::move(obj));
    return true;
}
auto PdfPage::db_deleteObject(int id) -> bool {
    if (id >= objs.size()) {
        return false;
    }
    delete_staging.emplace_back(id);
    return true;
}
// might be bad if dupes get inserted
auto PdfPage::db_insertObject(const StringObject& obj) -> void { insert_staging.emplace_back(obj); }
auto PdfPage::db_clear_staging() -> void {
    update_staging.clear();
    insert_staging.clear();
    delete_staging.clear();
}
auto PdfPage::db_commit() -> bool {
    for (auto& pair : update_staging) {
        const auto& orig = objs[pair.first];
        if (auto pos = raw.find(orig.toString()); pos != std::string::npos) {
            raw.replace(pos, orig.toString().size(),
                        "\n" + pair.second.getColor().toString() + "\n" + pair.second.toString() + orig.getColor().toString() + "\n");
            objs[pair.first] = std::move(pair.second);
        } else {
            db_clear_staging();
            return false;
        }
    }
    for (auto& obj : insert_staging) {
        raw.append("\n" + obj.getColor().toString() + "\n" + obj.toString());
        objs.emplace_back(std::move(obj));
    }
    std::sort(delete_staging.begin(), delete_staging.end(), std::greater<>());
    delete_staging.erase(std::unique(delete_staging.begin(), delete_staging.end()), delete_staging.end());
    for (auto& obj_id : delete_staging) {
        if (auto pos = raw.find(objs[obj_id].toString()); pos != std::string::npos) {
            raw.replace(pos, objs[obj_id].toString().size(), "");
            objs.erase(objs.begin() + obj_id);
        } else {
            db_clear_staging();
            return false;
        }
    }
    db_clear_staging();
    return true;
}
} // namespace sru::pdf