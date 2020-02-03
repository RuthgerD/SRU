#include "string_obj.h"
#include <string>
namespace sru::pdf {
StringObject::StringObject(sru::util::Color color, int text_type, double font_size, std::string transform, sru::util::Cordinate position,
                           std::string content)
    : color{color}, text_type{text_type}, font_size{font_size}, transform{transform}, position{position}, content{content} {}
const sru::util::Color& StringObject::getColor() const { return color; }
const int& StringObject::getTextType() const { return text_type; }
const double& StringObject::getFontSize() const { return font_size; }
const std::string& StringObject::getContent() const { return content; }
const sru::util::Cordinate& StringObject::getPosition() const { return position; }
const std::string StringObject::toString() const {
    // TODO: strip out leading 0s to better conform to pdf standard
    return "BT\n/F" + std::to_string(text_type) + " " + std::to_string(font_size) + " Tf\n" + transform + " " + std::to_string(position.getX()) +
           " " + std::to_string(position.getY()) + " Tm\n(" + content + ")Tj\nET\n";
}
bool& StringObject::isModified() { return modified; }

} // namespace sru::pdf
