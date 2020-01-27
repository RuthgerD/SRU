#include "pdf_file.h"
#include "pdf_page.h"
#include <vector>

namespace sru::pdf {
const std::vector<sru::pdf::PdfPage> &PdfFile::getPages() const {
    return pages;
}
} // namespace sru::pdf