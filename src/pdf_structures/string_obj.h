#ifndef _SRU_STRING_OBJ_
#define _SRU_STRING_OBJ_
#include "../util/util.h"
#include <string>

namespace sru::pdf {
class StringObject {
    sru::util::Cordinate position;
    std::string content;

  public:
    StringObject(sru::util::Cordinate position, std::string content)
        : position{position}, content{content} {};
    const std::string &getContent() const;
    const sru::util::Cordinate &getPosition() const;
};

};     // namespace sru::pdf
#endif // !_SRU_STRING_OBJ_
