#include "page_config.h"
#include <optional>
#include <vector>

namespace sru::pdf {
std::vector<PageConfig> PageConfigPool;
// const std::optional<PageConfig> getPageConfig(int id) {
//     for (const auto conf : PageConfigPool) {
//         if (conf.id == id) {
//             return conf;
//         }
//     }
//     return {};
// }
} // namespace sru::pdf