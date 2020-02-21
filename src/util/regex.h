#pragma once
#include "util.h"
#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/range/combine.hpp>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>
#include <cmath>
#include <iostream>
#include <fmt/format.h>
#include "re_accel.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <re2/re2.h>
#include <type_traits>

namespace sru::re {
auto re_search(const std::string& re, std::string_view data, int limit = -1) -> std::optional<std::vector<std::vector<std::string_view>>>;
auto re_match(const std::string& re, std::string_view data) -> bool;
auto re_replace(const std::string& re, std::string_view repl, std::string& data) -> bool;
auto re_group_count(const std::string& re) -> size_t;
} // namespace sru::re
