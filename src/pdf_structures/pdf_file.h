#pragma once
#include "../util/re_accel.h"
#include "../util/util.h"
#include "anchor_config.h"
#include "pdf_page.h"
#include "string_obj.h"
#include <boost/regex.hpp>
#include <chrono>
#include <ctll.hpp>
#include <ctre.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace sru::pdf {
static float timepool = 0;
class PdfFile {
    std::filesystem::path path;
    std::vector<PdfPage> pages;

  public:
    PdfFile(std::filesystem::path path);
    const std::filesystem::path& getPath() const;
    const std::vector<PdfPage>& getPages() const;
};
}; // namespace sru::pdf