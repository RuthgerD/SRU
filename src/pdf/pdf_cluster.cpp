#include "pdf_cluster.h"
#include "calc_config.h"

namespace sru::pdf {
PdfCluster::PdfCluster(std::vector<std::filesystem::path> pdf_file_paths) {
    std::vector<std::future<std::optional<sru::pdf::PdfFile>>> result;
    for (size_t i = 0; i < pdf_file_paths.size() - 1; i++) {
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
            const auto& a_date = a.getPage(a_found.front().first).getObject(a_found.front().second.front());
            const auto& b_date = b.getPage(b_found.front().first).getObject(b_found.front().second.front());
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
    for (auto& x : pdf_files_.at(0).getPages()) {
        //x.second.printObjects();
    }

    auto final_pdf = pdf_files_.front();

    for (auto& [page_no, page] : final_pdf.getPages()) {
        for (auto& [anchor_conf_id, val] : page.getAnchorPositions()) {
            auto anchor_conf = *getAnchorConfig(anchor_conf_id); // if the page got it then we dont need to check :)
            for (auto& object_conf_id : anchor_conf.sub_groups) {
                auto object_conf_opt = getObjectConfig(object_conf_id);
                if (!object_conf_opt) {
                    continue;
                }
                auto& object_conf = *object_conf_opt;

                auto new_objs = calculateObject(object_conf);

                if (new_objs.empty()) {
                    continue;
                }
                const auto& anchor_positions = page.getAnchorPositions();
                const auto& anchor_objs = page.getAnchorObjects();
                if (auto anchor_obj = anchor_objs.find(anchor_conf_id); anchor_obj != anchor_objs.end()) {
                    if(anchor_conf.content_ != anchor_conf.content_alt) {
                        auto new_anchor = page.getObject(anchor_obj->second);
                        new_anchor.setContent(anchor_conf.content_);
                        page.updateObject(anchor_obj->second, new_anchor);
                    }
                }
                if (auto anchor_pos = anchor_positions.find(anchor_conf_id);
                    anchor_pos != anchor_positions.end()) { // TODO: if no anchor found maybe find highest among objects
                    auto y_base = anchor_pos->second.getY();
                    float spacing = getCalcConfig(object_conf.calcs.front())->y_object_spacing; // TODO: maybe put this just on the object conf
                    for (auto& obj : new_objs) {
                        obj.setPosition(obj.getPosition().getX(), y_base + (spacing * ((int)(&obj - new_objs.data()) + 1.0)));
                    }
                }
                for (auto& obj : new_objs) {
                    page.insertObject(obj);
                }
                for (auto& obj : page.getMarkedObjects(object_conf_id)) {
                    page.deleteObject(obj);
                }
            }
        }
        page.commit();
    }
    /*
    std::vector<PdfPage> to_be;
    // NOT SORTED
    for (int i = 1; i < pdf_files_.size(); ++i) {
        for (const auto& x : pdf_files_[i].getPages()) {
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

auto PdfCluster::calccalc(const CalcConfig& cc, const std::vector<std::string>& contents, std::string reference) -> std::vector<std::string> {

    if (!(cc.calc_mode == "SUM" || cc.calc_mode == "SORT" || cc.calc_mode == "AVRG" || cc.calc_mode == "USER_INPUT" || cc.calc_mode == "FORWARD")) {
        return {};
    }

    if (cc.calc_mode == "FORWARD") {
        return {contents.front()};
    }

    auto [extracted_data, extracted_count] = sru::util::multi_search(cc.regex, contents, cc.re_extract_order);
    std::vector<std::string> new_content{};
    if (cc.calc_mode == "SUM") {
        const auto tmp = sru::util::multi_add(extracted_data, cc.overflow_threshold);

        std::vector<std::string> str_result;
        std::transform(tmp.begin(), tmp.end(), std::back_inserter(str_result),
                       [&](const auto& x) { return sru::util::to_string(x, cc.round_cut_off, cc.decimal_points); });

        if (!sru::util::multi_re_place(cc.regex, reference, str_result)) {
            return {};
        }
        new_content.push_back(std::move(reference));
    } else if (cc.calc_mode == "SORT") {
        const auto tmp = sru::util::multi_sort(extracted_data, contents, cc.sort_settings).second;

        std::move(tmp.begin(), (cc.maximum_values > tmp.size() ? tmp.end() : tmp.begin() + cc.maximum_values), std::back_inserter(new_content));
    } else if (cc.calc_mode == "AVRG") {
        std::vector<float> avrg_source;
        std::vector<float> avrg_base;
        if (cc.avrg_self) {
            avrg_source = sru::util::multi_add(extracted_count);
            avrg_base = sru::util::multi_add(extracted_data);
        } else {
            auto base_c = *getObjectConfig(cc.avrg_base_id.front());
            auto base_cc = *getCalcConfig(cc.avrg_base_id.back());
            auto source_c = *getObjectConfig(cc.avrg_source_id.front());
            auto source_cc = *getCalcConfig(cc.avrg_source_id.back());

            auto testing = calculateObject(base_c, base_cc);
            auto testing2 = calculateObject(source_c, source_cc);
            avrg_base = sru::util::multi_search(base_cc.regex, testing, base_cc.re_extract_order).first.front();
            avrg_source = sru::util::multi_search(source_cc.regex, testing2, source_cc.re_extract_order).first.front();
        }
        auto tmp = sru::util::multi_avrg(avrg_base, avrg_source, cc.avrg_multiplier);

        std::vector<std::string> tmp_s;
        std::transform(tmp.begin(), tmp.end(), std::back_inserter(tmp_s),
                       [&](const auto& x) { return sru::util::to_string(x, cc.round_cut_off, cc.decimal_points); });
        if (!sru::util::multi_re_place(cc.regex, reference, tmp_s)) {
            return {};
        }
        new_content.push_back(std::move(reference));
    }
    return new_content;
}

auto PdfCluster::calculateObject(const ObjectConfig& object_conf, const CalcConfig& calc_config) -> std::vector<std::string> {
    const auto total_objects = getMarkedObjects(object_conf.id, pdf_files_);
    auto reference = total_objects.front().getContent();

    std::vector<std::string> content;
    std::transform(total_objects.begin(), total_objects.end(), std::back_inserter(content), [](const auto& obj) { return obj.getContent(); });

    auto ret = calccalc(calc_config, content, reference);
    return ret;
}

auto PdfCluster::calculateObject(const ObjectConfig& object_conf) -> std::vector<StringObject> {
    auto total_objects = getMarkedObjects(object_conf.id, pdf_files_);
    if (total_objects.empty()) {
        return {};
    }
    auto reference = total_objects.front().getContent();

    std::vector<std::string> new_content{};
    for (const auto& calc_id : object_conf.calcs) {
        if (const auto calc_config_opt = getCalcConfig(calc_id); calc_config_opt) {
            const auto& calc_config = *calc_config_opt;

            std::vector<std::string> content;
            std::transform(total_objects.begin(), total_objects.end(), std::back_inserter(content), [](const auto& obj) { return obj.getContent(); });
            new_content = calccalc(calc_config, content, reference);
            if (!new_content.empty()) {
                std::cout << "Calculating: " << object_conf.name << " -> " << calc_config.name << std::endl;
                std::cout << "Result: " << new_content.front() << std::endl;
                reference = new_content.front();
            }
        }
    }
    std::vector<sru::pdf::StringObject> new_objects;
    if (!new_content.empty()) {
        for (const auto& j : new_content) {
            auto new_obj = total_objects.front();

            new_obj.setContent(j, 0);
            new_objects.push_back(std::move(new_obj));
        }
        return std::move(new_objects);
    }
    return {};
}
auto PdfCluster::getMarkedObjects(int id, std::vector<PdfFile>& files) -> std::vector<StringObject> {
    std::vector<StringObject> ret{};
    for (auto& file : files) {
        for (auto& pair : file.getPages()) {
            auto& page = pair.second;
            const auto& objs = page.getObjects();
            for (auto obj_id : page.getMarkedObjects(id)) {
                ret.emplace_back(objs[obj_id]);
            }
        }
    }
    return ret;
}
auto PdfCluster::refreshNumbering(PdfFile& file) -> void {

    std::vector<std::pair<ObjectConfig, CalcConfig>> numbering_confs{};

    for (const auto& conf : ObjectConfigPool) {
        if (conf.calcs.empty()) {
            continue;
        }
        if (const auto calc = getCalcConfig(conf.calcs.front()); calc) {
            if (calc->calc_mode == "NUMBERING") {
                numbering_confs.emplace_back(conf, *calc);
            }
        }
    }
    std::vector<int> count(numbering_confs.size());
    for (auto& [page_no, page] : file.getPages()) {
        for (size_t i = 0; i < numbering_confs.size(); ++i) {
            auto mrked_objs = page.getMarkedObjects(numbering_confs[i].first.id);
            for (auto id : mrked_objs) {
                auto copy = page.getObject(id);
                auto oldstr = copy.getContent();
                sru::util::multi_re_place(numbering_confs[i].second.regex, oldstr, {std::to_string(count[i] + 1)});
                copy.setContent(oldstr);
                page.updateObject(id, copy);
                ++count[i];
            }
        }
        page.commit();
    }
}
} // namespace sru::pdf