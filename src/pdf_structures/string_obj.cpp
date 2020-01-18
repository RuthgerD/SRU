#include "string_obj.h"
namespace sru::pdf {
const sru::util::Color &StringObject::getColor() const { return color; }
const int &StringObject::getTextType() const { return text_type; }
const double &StringObject::getFontSize() const { return font_size; }
const std::string &StringObject::getContent() const { return content; }
const sru::util::Cordinate &StringObject::getPosition() const {
    return position;
}
const std::string StringObject::toString() const {
    return "BT\n/F" + std::to_string(text_type) + " " +
           std::to_string(font_size) + " Tf\n" + transform + " " +
           std::to_string(position.getX()) + " " +
           std::to_string(position.getY()) + " Tm\n(" + content + ")Tj\nET\n";
}
bool &StringObject::isModified() { return modified; }

} // namespace sru::pdf
