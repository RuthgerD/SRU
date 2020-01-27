// #pragma once
// #include <boost/regex.hpp>
// #include <ctll.hpp>
// #include <ctre.hpp>
// #include <functional>
// #include <string>
// #include <string_view>
// #include <unordered_map>
// #include <utility>

// static constexpr auto pattern = ctll::fixed_string{
//     R"((?:([0-9\.]*) ([0-9\.]*) ([0-9\.]*) rg\n)|(?:BT\n/F([0-9]+?)
//     ([0-9.]+?) Tf\n([0-9. ]+) ([0-9.]+?) ([0-9.]+?)
//     Tm\n\((.+?)\)Tj\nET\n))"};
// static std::function<auto> testing = ctre::range<pattern>;
// static std::unordered_map<std::string, typeof(testing)> deed;

// static auto boost_pattern = boost::regex{
//     R"((?:([0-9\.]*) ([0-9\.]*) ([0-9\.]*) rg\n)|(?:BT\n/F([0-9]+?)
//     ([0-9.]+?) Tf\n([0-9. ]+) ([0-9.]+?) ([0-9.]+?)
//     Tm\n\((.+?)\)Tj\nET\n))"};