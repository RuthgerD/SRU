#include "string_obj.h"
#include <string>
#include <utility>
namespace sru::pdf {
StringObject::StringObject(sru::util::Color color, int text_type, double font_size, std::string transform, sru::util::Coordinate position,
                           std::string content)
    : color_{color}, text_type_{text_type}, font_size_{font_size}, transform_{std::move(transform)}, position_{position}, content_{
                                                                                                                              std::move(content)} {}
auto StringObject::getColor() const -> const sru::util::Color& { return color_; }
auto StringObject::getTextType() const -> int { return text_type_; }
auto StringObject::getFontSize() const -> double { return font_size_; }
auto StringObject::getContent() const -> const std::string& { return content_; }
auto StringObject::getPosition() const -> const sru::util::Coordinate& { return position_; }
auto StringObject::toString() const -> std::string {
    return "BT\n/F" + sru::util::to_string(text_type_, 0) + " " + sru::util::to_string(font_size_, 2) + " Tf\n" + transform_ + " " +
           sru::util::to_string(position_.getX(), 2) + " " + sru::util::to_string(position_.getY(), 2) + " Tm\n(" + content_ + ")Tj\nET\n";
}
auto StringObject::translateX(float amount) -> void { position_.translateX(amount); }
auto StringObject::translateY(float amount) -> void { position_.translateY(amount); }
auto StringObject::setContent(const std::string& new_content, float justify) -> void {
    translateX(((int)new_content.size() - (int)content_.size()) * justify);
    content_ = new_content;
}
// TODO: squash 0-256 to 0-1
auto StringObject::setColor(float r, float g, float b) -> void {
    color_.r_ = r;
    color_.g_ = g;
    color_.b_ = b;
}
auto StringObject::setTextType(int type) -> void { text_type_ = type; }
auto StringObject::setFontSize(float size) -> void { font_size_ = size; }
auto StringObject::setPosition(float x, float y) -> void { position_ = sru::util::Coordinate{x, y}; }
} // namespace sru::pdf
