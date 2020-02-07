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
    sru::util::Coordinate position;
    std::string content;

    bool modified = false;

  public:
    StringObject(sru::util::Color color, int text_type, double font_size, std::string transform, sru::util::Coordinate position, std::string content);

    [[nodiscard]] const sru::util::Color& getColor() const;
    [[nodiscard]] int getTextType() const;
    [[nodiscard]] double getFontSize() const;
    [[nodiscard]] auto getPosition() const -> const sru::util::Coordinate&;
    [[nodiscard]] auto getContent() const -> const std::string&;
    auto toString() const -> std::string;
    auto isModified() const -> bool;
};

}; // namespace sru::pdf
