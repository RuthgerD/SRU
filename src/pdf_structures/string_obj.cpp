#include "string_obj.h"
#include <string>
#include <utility>
namespace sru::pdf {
StringObject::StringObject(sru::util::Color color, int text_type, double font_size, std::string transform, sru::util::Coordinate position,
                           std::string content)
    : color{color}, text_type{text_type}, font_size{font_size}, transform{std::move(transform)}, position{position}, content{std::move(content)} {}
auto StringObject::getColor() const -> const sru::util::Color& { return color; }
auto StringObject::getTextType() const -> int { return text_type; }
auto StringObject::getFontSize() const -> double { return font_size; }
auto StringObject::getContent() const -> const std::string& { return content; }
auto StringObject::getPosition() const -> const sru::util::Coordinate& { return position; }
auto StringObject::toString() const -> std::string {
    return "BT\n/F" + sru::util::to_string(text_type, 0) + " " + sru::util::to_string(font_size, 2) + " Tf\n" + transform + " " +
           sru::util::to_string(position.getX(), 2) + " " + sru::util::to_string(position.getY(), 2) + " Tm\n(" + content + ")Tj\nET\n";
}
auto StringObject::translateX(float amount) -> void { position.translateX(amount); }
auto StringObject::translateY(float amount) -> void { position.translateY(amount); }
auto StringObject::setContent(const std::string& new_content, float justify) -> void {
    translateX(((int)new_content.size() - (int)content.size()) * justify);
    content = new_content;
}
// TODO: squash 0-256 to 0-1
auto StringObject::setColor(float r, float g, float b) -> void {
    color.r = r;
    color.g = g;
    color.b = b;
}
auto StringObject::setTextType(int type) -> void { text_type = type; }
auto StringObject::setFontSize(float size) -> void { font_size = size; }
auto StringObject::setPosition(float x, float y) -> void {
    position = sru::util::Coordinate{x,y};
}
} // namespace sru::pdf
