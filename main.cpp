#include <algorithm>
#include <cstdio>
#include <ctre.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

class pdfObject {
    double x;
    double y;
    std::string content;

  public:
    // omiting a lot of data for testing
    pdfObject(double x, double y, std::string content) {
        this->x = x;
        this->y = y;
        this->content = content;
    }

    std::string getContent() { return content; }
};

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
    std::vector<pdfObject> pdfObjects{};

    if (!pdfdata.has_value()) {
        std::cout << "File not found :)" << std::endl;
        return 1;
    }
    for (auto found : ctre::range<pattern>(pdfdata.value())) {
        pdfObjects.push_back(pdfObject{1.0, 1.0, found.get<4>().to_string()});
    }
    // toying with "functional" programming
    // std::for_each(pdfObjects.begin(), pdfObjects.end(),
    //               [](pdfObject obb) { std::cout << obb.getContent() << "\n";
    //               });

    // same thing but very cool
    std::vector<std::string> transformed{};
    std::transform(pdfObjects.begin(), pdfObjects.end(),
                   std::back_inserter(transformed),
                   [](pdfObject obb) { return obb.getContent(); });
    for (auto obb : transformed) {
        std::cout << obb << "\n";
    }
    return 0;
}
