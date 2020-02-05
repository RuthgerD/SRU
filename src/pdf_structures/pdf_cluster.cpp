#include "pdf_cluster.h"

namespace sru::pdf {
PdfCluster::PdfCluster(std::vector<std::filesystem::path> pdf_file_paths, sru::util::Qpdf qpdf) : qpdf{qpdf} {

    std::vector<std::future<PdfFile>> result;
    for (int i = 0; i < pdf_file_paths.size() - 1; i++) {
        const auto path = pdf_file_paths[i];
        result.push_back(std::async([path, &qpdf]() {
            const auto deflated = qpdf.decompress(path); // needs checks
            return PdfFile{deflated};
        }));
    }
    const auto deflated = qpdf.decompress(pdf_file_paths.back());

    pdf_files.emplace_back(deflated);

    for (auto& x : result) {
        auto tmp = x.get();
        pdf_files.push_back(std::move(tmp));
    }

    int index = 0;
    for (auto& x : pdf_files) {
        ++index;
        std::cout << "Pdf " << index << ":\n";
        for (const auto& page : x.getPages()) {
            // page.printObjects();
        }
    }
    calculate();
}
std::vector<std::reference_wrapper<sru::pdf::StringObject>> PdfCluster::getMarkedObjects(int id) const {
    std::vector<std::reference_wrapper<sru::pdf::StringObject>> total{};
    for (auto& file : pdf_files) {
        auto tmp = file.getMarkedObjects(id);
        total.insert(total.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
    }
    return total;
}

void PdfCluster::calculate() {

    for (const auto& anchor_conf : AnchorConfigPool) {
        std::cout << "Anchor: " << anchor_conf.name << std::endl;
        for (const auto& object_conf_id : anchor_conf.sub_groups) {
            if (const auto& object_conf = getObjectConfig(object_conf_id); object_conf) {
                const auto total_objects = getMarkedObjects(object_conf_id);
                if (total_objects.empty()) {
                    std::cout << "No objects found for " << object_conf->name << std::endl;
                    continue;
                }
                auto modes = object_conf->calc_modes;
                auto regexs = object_conf->regexs;
                if (modes.size() != regexs.size()) {
                    std::cout << "Not enough regexs supplied for " << object_conf->name << std::endl;
                    continue;
                }
                //

                for (std::size_t i = 0; i < modes.size(); i++) {
                    auto mode = modes[i];
                    auto regex = regexs[i];
                    std::vector<std::string> content;
                    std::transform(total_objects.begin(), total_objects.end(), std::back_inserter(content),
                                   [](const auto& obj) { return obj.get().getContent(); });
                    std::cout << "mode: " << mode << " regex: " << regex << std::endl;
                    auto extracted = sru::util::multi_search(regex, content, std::vector<int>{});
                    // more todo
                }
            } else {
                std::cout << "Obj conf id: " << object_conf_id << " not found." << std::endl;
            }
        }
    }
}
} // namespace sru::pdf