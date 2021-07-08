#ifndef _DOM_H_
#define _DOM_H_

#include "htmlStruct.h"

namespace berners {
    class Dom {
        public:
            Dom() {}
            ~Dom() {}

            static void printDom(const berners::Node& root, int d);
    };
};

#endif
