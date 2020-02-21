#include "pdf_cluster.h"

namespace sru::pdf {
PdfCluster::PdfCluster(std::vector<std::filesystem::path> pdf_file_paths) {
    std::vector<std::future<std::optional<sru::pdf::PdfFile>>> result;
    for (int i = 0; i < pdf_file_paths.size() - 1; i++) {
        const auto path = pdf_file_paths[i];
        result.push_back(std::async(std::launch::async, [path]() {
            if (const auto deflated_path_opt = sru::qpdf::decompress(path); deflated_path_opt) {
                const auto& deflated_path = *deflated_path_opt;
                if (const auto tmp_opt = sru::util::QFileRead(deflated_path); tmp_opt) {
                    const auto& tmp = *tmp_opt;
                    return std::optional{PdfFile{tmp, deflated_path}};
                }
            }
            return std::optional<sru::pdf::PdfFile>{};
        }));
    }
    if (const auto deflated_path_opt = sru::qpdf::decompress(pdf_file_paths.back()); deflated_path_opt) {
        const auto& deflated_path = *deflated_path_opt;
        if (const auto tmp_opt = sru::util::QFileRead(deflated_path); tmp_opt) {
            const auto& tmp = *tmp_opt;
            pdf_files_.emplace_back(tmp, deflated_path);
        }
    }
    for (auto& x : result) {
        if (auto tmp = x.get(); tmp) {
            pdf_files_.emplace_back(std::move(*tmp));
        }
    }
    std::sort(pdf_files_.begin(), pdf_files_.end(), [](PdfFile& a, PdfFile& b) {
        // TODO: dont hardcode
        auto DATE_FORMAT = "%d.%m.%Y %H:%M:%S";
        bool ret = false;
        auto a_found = a.getMarkedObjects(DATE_PROVIDER);
        auto b_found = b.getMarkedObjects(DATE_PROVIDER);

        if (!a_found.empty() && !b_found.empty()) {
            const auto& a_date = a.getPage(a_found.front().first).second.db_get(a_found.front().second.front());
            const auto& b_date = b.getPage(b_found.front().first).second.db_get(b_found.front().second.front());
            auto tp1 = sru::util::strptime(a_date.getContent(), DATE_FORMAT);
            auto tp2 = sru::util::strptime(b_date.getContent(), DATE_FORMAT);
            if (tp1 && tp2) {
                ret = *tp1 < *tp2;
            }
        }
        return ret;
    });
}
auto PdfCluster::exportTest() -> void {
    for (auto& x : pdf_files_.at(2).getPages()) {
        x.second.printObjects();
    }

    auto final_pdf = pdf_files_.front();

    for (auto& [page_no, page] : final_pdf.getPages()) {
        for (auto& [anchor_conf_id, val] : page.getAnchorPositions()) {
            auto anchor_conf = *getAnchorConfig(anchor_conf_id); // if the page got it then we dont need to check :)
            for (auto& object_conf_id : anchor_conf.sub_groups) {
                auto object_conf = *getObjectConfig(object_conf_id);
                auto total_objs = getMarkedObjects(object_conf_id, pdf_files_); // get ALL objects from all the files that follow the same config

                auto new_objs = calculateObject(object_conf, total_objs);

                if (!new_objs.empty()) {
                    const auto& anchor_positions = page.getAnchorPositions();
                    if (auto anchor_pos = anchor_positions.find(anchor_conf_id);
                        anchor_pos != anchor_positions.end()) { // if no anchor found maybe find highest among objects
                        auto y_base = anchor_pos->second.getY();
                        float offset = object_conf.y_object_spacing;
                        for (auto& obj : new_objs) {
                            obj.setPosition(obj.getPosition().getX(), y_base + (offset * ((int)(&obj - new_objs.data()) + 1)));
                        }
                    }
                    for (auto& obj : new_objs) {
                        page.db_insertObject(obj);
                    }
                    for (auto& obj : page.db_getMarkedObjects(object_conf_id)) {
                        page.db_deleteObject(obj);
                    }
                }
            }
        }
        page.db_commit();
    }

    /*
    std::vector<PdfPage> to_be;
    // NOT SORTED
    for (int i = 1; i < pdf_files.size(); ++i) {
        for (const auto& x : pdf_files[i].getPages()) {
            if (x.second.getConfig().mutate_in_final == "append") {
                to_be.push_back(x.second);
            }
        }
    }

    final_pdf.insertPages(std::move(to_be), 999);
    */
    refreshNumbering(final_pdf);

    const auto raw = final_pdf.getRaw();
    sru::util::QFileWrite(raw, std::filesystem::current_path().append("testing_export.pdf"));
}

auto PdfCluster::calculateObject(const ObjectConfig& object_conf, const std::vector<StringObject>& total_objects) -> std::vector<StringObject> {
    if (total_objects.empty()) {
        std::cout << "No objects found for " << object_conf.name << std::endl;
        return {};
    }
    auto modes = object_conf.calc_modes;
    auto regexs = object_conf.regexs;
    if (modes.size() != regexs.size()) {
        std::cout << "Not enough regexs supplied for " << object_conf.name << std::endl;
        return {};
    }

    std::vector<std::string> new_content;
    std::vector<sru::pdf::StringObject> provided_objects;
    auto reference = total_objects.front().getContent();
    for (int i = 0; i < modes.size(); i++) {
        auto mode = modes[i];
        auto regex = regexs[i];

        const auto& cut_off = object_conf.round_cut_off;
        const auto& decimal_points = object_conf.decimal_points;
        const auto& overflow_threshold = object_conf.overflow_threshold;

        std::vector<std::string> content;
        std::transform(total_objects.begin(), total_objects.end(), std::back_inserter(content), [](const auto& obj) { return obj.getContent(); });

        auto extracted = sru::util::multi_search(regex, content, object_conf.re_extract_order);
        if (extracted.first.empty()) {
            break;
        }

        if (mode == "ADD") {
            auto result = sru::util::multi_add(extracted.first, overflow_threshold);

            std::vector<std::string> str_result;
            std::transform(result.begin(), result.end(), std::back_inserter(str_result),
                           [cut_off, decimal_points](const auto& x) { return sru::util::to_string(x, cut_off, decimal_points); });

            if (!sru::util::multi_re_place(regex, reference, str_result)) {
                break;
            }
            new_content.push_back(reference);
        }
        if (mode == "SORT") {
            const auto& limit = object_conf.maximum_values;
            const auto& sort_settings = object_conf.sort_settings;
            auto tmp = sru::util::multi_sort(extracted.first, total_objects, sort_settings).second;

            for (int k = 0; k < tmp.size() && k < limit; ++k) {
                provided_objects.push_back(tmp[k]);
                new_content.push_back(tmp[k].getContent());
            }
        }
        if (mode == "AVRG") {
            const auto avrg_source_id = object_conf.avrg_source_id;
            const auto avrg_base_id = object_conf.avrg_base_id;
            const auto& avrg_self = object_conf.avrg_self;

            std::vector<float> avrg_source;
            std::vector<float> avrg_base;
            if (!avrg_self) {
                // TODO: recurse call to calculate dependents
                /*
                if (calculated.find(avrg_source_id) == calculated.end() || calculated.find(avrg_base_id) == calculated.end()) {
                    break;
                }
                avrg_source = calculated[avrg_source_id];
                avrg_base = calculated[avrg_base_id];
                 */
            } else {
                avrg_source = sru::util::multi_add(extracted.second);
                avrg_base = sru::util::multi_add(extracted.first);
            }

            const auto& multiplier = object_conf.avrg_multiplier;
            auto result = sru::util::multi_avrg(avrg_source, avrg_base, multiplier);

            std::vector<std::string> str_result;
            std::transform(result.begin(), result.end(), std::back_inserter(str_result),
                           [cut_off, decimal_points](const auto& x) { return sru::util::to_string(x, cut_off, decimal_points); });
            if (!sru::util::multi_re_place(regex, reference, str_result)) {
                break;
            }
            new_content.push_back(reference);
        }
        if (mode == "USER_INPUT") {
            // TODO: implement
        }
        if (!new_content.empty()) {
            reference = new_content.front();
        }
    }

    std::vector<sru::pdf::StringObject> new_objects;
    if (!new_content.empty()) {
        if (modes.size() > 1) {
            new_content = {new_content.back()};
        }
        for (int j = 0; j < new_content.size(); ++j) {
            const sru::pdf::StringObject* tmp;
            if (provided_objects.empty()) {
                tmp = &total_objects.front();
            } else {
                tmp = &provided_objects[j];
            }

            sru::pdf::StringObject new_obj = *tmp;

            new_obj.setContent(new_content[j], object_conf.text_justify);
            new_objects.push_back(std::move(new_obj));
        }
        return new_objects;
    }
    return {};
}
auto PdfCluster::getMarkedObjects(int id, std::vector<PdfFile>& files) -> std::vector<StringObject> {
    std::vector<StringObject> ret{};
    for (auto& file : files) {
        for (auto& pair : file.getPages()) {
            auto& page = pair.second;
            const auto& objs = page.db_getObjects();
            for (auto obj_id : page.db_getMarkedObjects(id)) {
                ret.emplace_back(objs[obj_id]);
            }
        }
    }
    return ret;
}
auto PdfCluster::refreshNumbering(PdfFile& file) -> void {
    std::vector<ObjectConfig> numbering_confs{};
    for (const auto& conf : ObjectConfigPool) {
        if (conf.calc_modes.size() == 1 && conf.regexs.size() == 1) {
            if (conf.calc_modes.front() == "NUMBERING") {
                numbering_confs.push_back(conf);
            }
        }
    }
    std::vector<int> count(numbering_confs.size());
    for (auto& [page_no, page] : file.getPages()) {
        for (int i = 0; i < numbering_confs.size(); ++i) {
            auto mrked_objs = page.db_getMarkedObjects(numbering_confs[i].id);
            for (auto id : mrked_objs) {
                auto copy = page.db_get(id);
                auto oldstr = copy.getContent();
                sru::util::multi_re_place(numbering_confs[i].regexs[0], oldstr, {std::to_string(count[i] + 1)});
                copy.setContent(oldstr);
                page.db_updateObject(id, copy);
                ++count[i];
            }
        }
        page.db_commit();
    }
}
} // namespace sru::pdf