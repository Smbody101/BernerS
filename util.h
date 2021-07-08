#ifndef _UTIL_H_
#define _UTIL_H_

#include <vector>
#include <string>

namespace berners{
    class Util {
        public:
            static std::vector<std::string> split(const std::string& s, char separator);
    };
};

#endif