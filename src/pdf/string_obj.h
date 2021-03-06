#pragma once

#include "../util/util.h"
#include <iostream>
#include <string>

namespace sru::pdf {
class StringObject {
    sru::util::Color color_;
    int text_type_;
    double font_size_;
    std::string transform_; // only used to reconstruct original atm.
    sru::util::Coordinate position_;
    std::string content_;

  public:
    StringObject(sru::util::Color color, int text_type, double font_size, std::string transform, sru::util::Coordinate position, std::string content);
    inline StringObject(const StringObject& other) = default;
    inline ~StringObject() = default;
    [[nodiscard]] auto getColor() const -> const sru::util::Color&;
    [[nodiscard]] auto getTextType() const -> int;
    [[nodiscard]] auto getFontSize() const -> double;
    [[nodiscard]] auto getPosition() const -> const sru::util::Coordinate&;
    [[nodiscard]] auto getContent() const -> const std::string&;
    [[nodiscard]] auto toString() const -> std::string;
    auto setColor(float r, float g, float b) -> void;
    auto setColor(sru::util::Color& color) -> void;
    auto setContent(const std::string& new_content, float justify = 0) -> void;
    auto setTextType(int type) -> void;
    auto setFontSize(float size) -> void;
    auto translateX(float amount) -> void;
    auto translateY(float amount) -> void;
    auto setPosition(float x, float y) -> void;
};

} // namespace sru::pdf
