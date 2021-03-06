#include "pdf_page.h"
#include "../util/util.h"
#include "anchor_config.h"
#include "object_config.h"
#include <cmath>
#include <utility>
#include <vector>

namespace sru::pdf {
PdfPage::PdfPage(std::string raw, const PageConfig& config) : raw_{std::move(raw)}, config_{config} {}
auto PdfPage::getConfig() const -> PageConfig { return config_; }
auto PdfPage::getAnchorPositions() const -> const std::unordered_map<int, sru::util::Coordinate>& { return anchor_positions_; }
auto PdfPage::getAnchorObjects() const -> const std::unordered_map<int, Offset>& { return anchor_objs_; }
void PdfPage::indexObjects() {
    objs_.clear();
    marked_objs_.clear();
    anchor_objs_.clear();
    clear_staging();
    if (const auto found = sru::re::re_search(config_.obj_regex, raw_); found) {
        auto color = sru::util::Color{0, 0, 0};
        for (const auto& x : *found) {
            objs_.emplace_back(color, *sru::util::svto<int>(x[1]), *sru::util::svto<float>(x[2]), std::string{x[3]},
                               sru::util::Coordinate{*sru::util::svto<float>(x[4]), *sru::util::svto<float>(x[5])}, std::string{x[6]});
        }
    }
    for (auto& obj : objs_) {
        for (auto anchor_conf_id : config_.groups) {
            if (const auto anchor_conf_opt = getAnchorConfig(anchor_conf_id); anchor_conf_opt) {
                const auto& anchor_conf = *anchor_conf_opt;
                auto conf_x = anchor_conf.position.getX();
                auto conf_y = anchor_conf.position.getY();
                if (anchor_conf.is_virtual || (std::fabs(conf_y) / conf_y) * obj.getPosition().getY() >= conf_y ||
                    (std::fabs(conf_x) / conf_x) * obj.getPosition().getX() <= conf_x ||
                    !sru::re::re_match(anchor_conf.content_id, obj.getContent())) {
                    continue;
                }
                if (anchor_conf.save_anchor) {
                    anchor_objs_.emplace(anchor_conf_id, &obj - objs_.data());
                }
                anchor_positions_.emplace(anchor_conf_id, obj.getPosition());
            }
        }
    }
    for (const auto& anchor_conf_id : config_.groups) {
        if (const auto anchor_conf = getAnchorConfig(anchor_conf_id); anchor_conf) {
            if (anchor_conf->is_virtual) {
                anchor_positions_.emplace(anchor_conf_id, anchor_conf->position);
            }
        }
    }
    for (const auto& [anchor_conf_id, anchor_position] : anchor_positions_) {
        if (const auto anchor_conf = getAnchorConfig(anchor_conf_id); anchor_conf) {
            const auto& anchor_obj = anchor_position;
            auto count_start = 0;
            auto prev_margin_x = 0;
            auto prev_margin_y = 0;

            for (auto object_conf_id : anchor_conf->sub_groups) {
                if (const auto object_conf_opt = getObjectConfig(object_conf_id); object_conf_opt) {
                    const auto& object_conf = *object_conf_opt;

                    auto ref_x = anchor_obj.getX();
                    auto ref_y = anchor_obj.getY();

                    auto max_x = ref_x + object_conf.margin_x;
                    auto max_y = ref_y + object_conf.margin_y;

                    if (auto anchor_margin = anchor_positions_.find(object_conf.anchor_margin_x); anchor_margin != anchor_positions_.end()) {
                        max_x = anchor_margin->second.getX();
                    }
                    if (auto anchor_margin = anchor_positions_.find(object_conf.anchor_margin_y); anchor_margin != anchor_positions_.end()) {
                        max_y = anchor_margin->second.getY();
                    }

                    const auto x1 = ref_x < max_x ? ref_x : max_x;
                    const auto y1 = ref_y > max_y ? ref_y : max_y;

                    const auto x2 = ref_x > max_x ? ref_x : max_x;
                    const auto y2 = ref_y < max_y ? ref_y : max_y;

                    const auto object_count = object_conf.object_count;

                    auto found_count = 0;
                    auto captured_count = 0;

                    for (auto& comp_obj : objs_) {
                        const auto comp_x = std::fabs(comp_obj.getPosition().getX());
                        const auto comp_y = std::fabs(comp_obj.getPosition().getY());

                        if (((y1 >= comp_y && x1 < comp_x && x2 > comp_x && y2 <= comp_y) ||
                             (y1 > comp_y && x1 <= comp_x && x2 >= comp_x && y2 < comp_y)) ||
                            (comp_x == ref_x && comp_y == ref_y && !anchor_conf->save_anchor)) {
                            if (captured_count == object_count) {
                                break;
                            }
                            // small hack where we reset count_start if it searches in a different direction, TODO: find something better
                            if (prev_margin_x != object_conf.margin_x && prev_margin_y != object_conf.margin_y) {
                                count_start = 0;
                            }
                            if (found_count >= count_start) {
                                if (marked_objs_.find(object_conf_id) == marked_objs_.end()) {
                                    marked_objs_.emplace(object_conf_id, std::vector<Offset>{});
                                }
                                marked_objs_[object_conf_id].emplace_back(&comp_obj - objs_.data());
                                ++captured_count;
                            }
                            ++found_count;
                            prev_margin_x = object_conf.margin_x;
                            prev_margin_y = object_conf.margin_x;
                        }
                    }
                    count_start += object_count;
                }
            }
        }
    }
}
void PdfPage::printObjects() const {
    for (auto anchor_pair : anchor_positions_) {
        if (auto conf = getAnchorConfig(anchor_pair.first)) {
            std::cout << conf.value().name << ":\n";
            for (auto id : conf.value().sub_groups) {
                if (marked_objs_.find(id) != marked_objs_.end()) {
                    for (auto& obj : marked_objs_.at(id)) {
                        std::cout << "* " << objs_[obj].getContent() << "\n";
                    }
                }
            }
        }
    }
}
auto PdfPage::getRaw() const -> const std::string& { return raw_; }

auto PdfPage::getObjects() -> const std::vector<StringObject>& { return objs_; }
auto PdfPage::getObject(Offset id) const -> const StringObject& {
    assert(id < objs_.size());
    return objs_[id];
}
auto PdfPage::getMarkedObjects(size_t id) -> std::vector<Offset> {
    if (marked_objs_.find(id) != marked_objs_.end()) {
        return marked_objs_[id];
    }
    return {};
}
auto PdfPage::updateObject(Offset id, StringObject& obj) -> bool {
    if (id > objs_.size()) {
        return false;
    }
    update_staging_.erase(id);
    update_staging_.emplace(id, std::move(obj));
    return true;
}
auto PdfPage::deleteObject(Offset id) -> bool {
    if (id > objs_.size()) {
        return false;
    }
    delete_staging_.emplace_back(id);

    return true;
}
// might be bad if dupes get inserted
auto PdfPage::insertObject(StringObject& obj) -> void { insert_staging_.emplace_back(std::move(obj)); }
auto PdfPage::clear_staging() -> void {
    update_staging_.clear();
    insert_staging_.clear();
    delete_staging_.clear();
}
auto PdfPage::commit() -> bool {
    for (auto& [id, obj] : update_staging_) {
        const auto& orig = objs_[id];
        if (auto pos = raw_.find(orig.toString()); pos != std::string::npos) {
            raw_.replace(pos, orig.toString().size(), "");
            raw_.append("\n" + obj.getColor().toString() + "\n" + obj.toString());
            objs_[id] = std::move(obj);
        } else {
            clear_staging();
            return false;
        }
    }
    for (auto& obj : insert_staging_) {
        raw_.append("\n" + obj.getColor().toString() + "\n" + obj.toString());
        objs_.emplace_back(std::move(obj));
    }

    for (auto id : delete_staging_) {
        sru::util::erase_if(marked_objs_, [&](auto& key, auto& val) {
            for (auto& x : val) {
                if (x == id) {
                    val.erase(val.begin() + (&x - val.data()));
                }
            }
            return val.empty();
        });
        sru::util::erase_if(anchor_objs_, [&](auto& key, auto& val) { return val == id; });
    }
    //
    std::sort(delete_staging_.begin(), delete_staging_.end(), std::greater<>());
    delete_staging_.erase(std::unique(delete_staging_.begin(), delete_staging_.end()), delete_staging_.end());

    auto delta_adj = [&](auto& map) {
        for (auto& [key, val] : map) {
            for (auto& x : val)
                x -= std::distance(delete_staging_.begin(), std::upper_bound(delete_staging_.begin(), delete_staging_.end(), x));
        }
    };
    delta_adj(marked_objs_);
    for (auto& [key, val] : anchor_objs_) {
        val -= std::distance(delete_staging_.begin(), std::upper_bound(delete_staging_.begin(), delete_staging_.end(), val));
    }

    for (auto& obj_id : delete_staging_) {
        if (auto pos = raw_.find(objs_[obj_id].toString()); pos != std::string::npos) {
            raw_.replace(pos, objs_[obj_id].toString().size(), "");
            objs_.erase(objs_.begin() + obj_id);
        } else {
            clear_staging();
            return false;
        }
    }
    clear_staging();
    return true;
}
} // namespace sru::pdf