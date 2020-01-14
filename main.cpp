#include <cstdio>
#include <ctre.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

// Test pattern to capture uncompressed pdf objects
static constexpr auto pattern = ctll::fixed_string{
    "BT\\n/F(.*?) (.*?) Tf\\n(.*?) Tm\\n\\((.*?)\\)Tj\\nET\\n"};

std::optional<std::string> QFileRead(const char *path) {
    if (auto f = std::fopen(path, "r")) {
        std::fseek(f, 0, SEEK_END);
        std::string str;
        str.resize(std::ftell(f));
        std::fseek(f, 0, SEEK_SET);
        std::fread(str.data(), str.length(), 1, f);
        std::fclose(f);
        return str;
    } else {
        return {};
    }
}

int main(int argc, char **argv) {
    const auto pdfdata = QFileRead("/home/ruthger/test.pdf");

    if (pdfdata.has_value()) {
        for (auto found : ctre::range<pattern>(pdfdata.value())) {
            std::cout << found.get<0>().to_view();
        }
    } else {
        std::cout << "File not found :)" << std::endl;
    }
}
