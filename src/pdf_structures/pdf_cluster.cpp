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
            pdf_files.emplace_back(tmp, deflated_path);
        }
    }
    for (auto& x : result) {
        if (auto tmp = x.get(); tmp) {
            pdf_files.emplace_back(std::move(*tmp));
        }
    }
    std::sort(pdf_files.begin(), pdf_files.end(), [](PdfFile& a, PdfFile& b) {
        bool ret = false;
        auto a_date = a.getMarkedObjects(DATE_PROVIDER);
        auto b_date = b.getMarkedObjects(DATE_PROVIDER);
        if (a_date.size() > 0 && a_date.size() > 0) {
            // TODO: dont hardcode format and move to own function strptime()
            std::tm tm1 = {};
            std::stringstream ss1(a_date.front().get().getContent());

            std::tm tm2 = {};
            std::stringstream ss2(b_date.front().get().getContent());
            ss1 >> std::get_time(&tm1, "%d.%m.%Y %H:%M:%S");

            ss2 >> std::get_time(&tm2, "%d.%m.%Y %H:%M:%S");
            if (!ss2.fail() && !ss1.fail()){
                auto tp1 = std::chrono::system_clock::from_time_t(std::mktime(&tm1));
                auto tp2 = std::chrono::system_clock::from_time_t(std::mktime(&tm2));
                ret = tp1 < tp2;
            }
        }
        return ret;
    });
    calculate();
}
auto PdfCluster::exportTest() -> void {
    sru::util::QFileWrite(pdf_files.front().getRaw(), std::filesystem::current_path().append("testing_export.pdf"));
}

auto PdfCluster::getMarkedObjects(int id) -> std::vector<std::reference_wrapper<sru::pdf::StringObject>> {
    std::vector<std::reference_wrapper<sru::pdf::StringObject>> total{};
    for (auto& file : pdf_files) {
        auto tmp = file.getMarkedObjects(id);
        total.insert(total.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
    }
    return total;
}

auto PdfCluster::calculate() -> std::unordered_map<int, std::vector<sru::pdf::StringObject>> {
    std::unordered_map<int, std::vector<float>> calculated;
    std::unordered_map<int, std::vector<sru::pdf::StringObject>> new_objects_map;

    for (const auto& anchor_conf : AnchorConfigPool) {
        std::cout << "Anchor: " << anchor_conf.name << std::endl;
        for (const auto& object_conf_id : anchor_conf.sub_groups) {
            if (const auto& object_conf_opt = getObjectConfig(object_conf_id); object_conf_opt) {
                const auto& object_conf = *object_conf_opt;
                const auto total_objects = getMarkedObjects(object_conf_id);
                if (total_objects.empty()) {
                    std::cout << "No objects found for " << object_conf.name << std::endl;
                    continue;
                }
                auto modes = object_conf.calc_modes;
                auto regexs = object_conf.regexs;
                if (modes.size() != regexs.size()) {
                    std::cout << "Not enough regexs supplied for " << object_conf.name << std::endl;
                    continue;
                }

                std::vector<std::string> new_content;
                std::vector<std::reference_wrapper<sru::pdf::StringObject>> provided_objects;
                const auto& reference_object = total_objects.front();
                auto reference = total_objects.front().get().getContent();
                for (int i = 0; i < modes.size(); i++) {
                    auto mode = modes[i];
                    auto regex = regexs[i];

                    const auto& cut_off = object_conf.round_cut_off;
                    const auto& decimal_points = object_conf.decimal_points;
                    const auto& overflow_threshold = object_conf.overflow_threshold;

                    std::vector<std::string> content;
                    std::transform(total_objects.begin(), total_objects.end(), std::back_inserter(content),
                                   [](const auto& obj) { return obj.get().getContent(); });

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
                            new_content.push_back(tmp[k].get().getContent());
                        }
                    }
                    if (mode == "AVRG") {
                        const auto avrg_source_id = object_conf.avrg_source_id;
                        const auto avrg_base_id = object_conf.avrg_base_id;
                        const auto& avrg_self = object_conf.avrg_self;

                        std::vector<float> avrg_source;
                        std::vector<float> avrg_base;
                        if (!avrg_self) {
                            if (calculated.find(avrg_source_id) == calculated.end() || calculated.find(avrg_base_id) == calculated.end()) {
                                break;
                            }
                            avrg_source = calculated[avrg_source_id];
                            avrg_base = calculated[avrg_base_id];
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

                    reference = new_content.front();
                }

                std::vector<sru::pdf::StringObject> new_objects;
                if (!new_content.empty()) {
                    if (modes.size() > 1) {
                        new_content = {new_content.back()};
                    }
                    for (int j = 0; j < new_content.size(); ++j) {
                        const std::reference_wrapper<sru::pdf::StringObject>* tmp = nullptr;
                        if (provided_objects.empty()) {
                            tmp = &reference_object;
                        } else {
                            tmp = &provided_objects[j];
                        }
                        sru::pdf::StringObject new_obj{tmp->get()};

                        new_obj.setContent(new_content[j], object_conf.text_justify);
                        new_objects.push_back(std::move(new_obj));
                    }
                    new_objects_map.emplace(object_conf_id, new_objects);
                }
            } else {
                std::cout << "Obj conf id: " << object_conf_id << " not found." << std::endl;
            }
        }
    }
    return new_objects_map;
}
} // namespace sru::pdf