#pragma once

#include "../util/util.h"
#include <iostream>
#include <string>

namespace sru::pdf {
class StringObject {
    sru::util::Color color;
    int text_type;
    double font_size;
    std::string transform; // only used to reconstruct original atm.
    sru::util::Cordinate position;
    std::string content;

    bool modified = false;

  public:
    StringObject(sru::util::Color color, int text_type, double font_size,
                 std::string transform, sru::util::Cordinate position,
                 std::string content);

    const sru::util::Color &getColor() const;
    const int &getTextType() const;
    const double &getFontSize() const;
    const sru::util::Cordinate &getPosition() const;
    const std::string &getContent() const;
    const std::string toString() const;
    bool &isModified();
};

}; // namespace sru::pdf
