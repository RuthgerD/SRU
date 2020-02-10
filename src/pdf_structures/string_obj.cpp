#include "string_obj.h"
#include <string>
#include <utility>
namespace sru::pdf {
StringObject::StringObject(sru::util::Color color, int text_type, double font_size, std::string transform, sru::util::Coordinate position,
                           std::string content)
    : color{color}, text_type{text_type}, font_size{font_size}, transform{std::move(transform)}, position{position}, content{std::move(content)} {}
const sru::util::Color& StringObject::getColor() const { return color; }
int StringObject::getTextType() const { return text_type; }
double StringObject::getFontSize() const { return font_size; }
const std::string& StringObject::getContent() const { return content; }
const sru::util::Coordinate& StringObject::getPosition() const { return position; }
std::string StringObject::toString() const {
    // TODO: strip out leading 0s to better conform to pdf standard
    return "BT\n/F" + std::to_string(text_type) + " " + std::to_string(font_size) + " Tf\n" + transform + " " + std::to_string(position.getX()) +
           " " + std::to_string(position.getY()) + " Tm\n(" + content + ")Tj\nET\n";
}
bool StringObject::isModified() const { return modified; }
auto StringObject::translateX(float amount) -> void { position.translateX(amount); }
auto StringObject::translateY(float amount) -> void { position.translateY(amount); }
auto StringObject::setContent(const std::string& new_content, float justify) -> void {
    translateX(new_content.size() - content.size());
    content = new_content;
}
} // namespace sru::pdf
