#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <cstdint>
#include <sstream>

namespace Lucene {

typedef std::string String;
const String EmptyString;

typedef std::basic_ostringstream< char, std::char_traits<char> > StringStream;

class StringUtils {
public:
    template <class TYPE>
    static String to_string(const TYPE& value) {
        StringStream os;
        os << value;
        return os.str();
    }
};

} // namespace Lucene

#endif // STRING_UTILS_H
