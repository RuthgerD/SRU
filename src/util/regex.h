#pragma once
#include "re_accel.h"
#include "util.h"
#include <algorithm>
#include <array>
#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/range/combine.hpp>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <re2/re2.h>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace sru::re {
auto re_search(const std::string& re, std::string_view data, int limit = -1) -> std::optional<std::vector<std::vector<std::string_view>>>;
auto re_match(const std::string& re, std::string_view data) -> bool;
auto re_replace(const std::string& re, std::string_view repl, std::string& data) -> bool;
auto re_group_count(const std::string& re) -> size_t;
} // namespace sru::re
