#include "string_obj.h"
namespace sru::pdf {
const std::string &StringObject::getContent() const { return content; }
const sru::util::Cordinate &StringObject::getPosition() const {
    return position;
}

} // namespace sru::pdf
