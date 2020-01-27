#include "pdf_cluster.h"
#include "rapidjson/document.h"

namespace sru::pdf {
const rapidjson::Document &PdfCluster::getConfig() const { return config; }
}