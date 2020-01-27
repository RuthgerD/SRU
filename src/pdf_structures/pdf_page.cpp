#include "pdf_page.h"
#include <vector>

namespace sru::pdf {
const std::vector<sru::pdf::StringObject> &PdfPage::getObjects() const {
    return objs;
}
} // namespace sru::pdf