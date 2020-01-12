#include <cstdio>
#include <ctre.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

// Test pattern to capture uncompressed pdf objects
static constexpr auto pattern = ctll::fixed_string{
    "BT\\n/F(.*?) (.*?) Tf\\n(.*?) Tm\\n\\((.*?)\\)Tj\\nET\\n"};

std::string QFileRead(const char *path) {
    auto f = std::fopen(path, "r");
    std::fseek(f, 0, SEEK_END);
    std::string str;
    str.resize(std::ftell(f));
    std::fseek(f, 0, SEEK_SET);
    std::fread(static_cast<char *>(str.data()), str.length(), 1, f);
    std::fclose(f);
    return str;
}

int main(int argc, char **argv) {
    const std::string pdfdata = QFileRead("/home/ruthger/test.pdf");

    for (auto found : ctre::range<pattern>(pdfdata)) {
        std::cout << found.get<0>().to_view();
    }
}
