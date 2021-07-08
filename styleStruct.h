#ifndef _STYLESTRUCT_H_
#define _STYLESTRUCT_H_
#include <iostream>
#include <unordered_map>
#include <initializer_list>
#include "cssStruct.h"
#include "htmlStruct.h"

namespace berners {
    typedef std::unordered_map<std::string, berners::Value> PropertyMap;

    struct MatchedRule{
        berners::Specificity specificity;
        berners::Rule rule;
        bool valid;
    };

    enum Display{
        Inline, Block, None
    };

    class StyledNode{
        public:
            static berners::Value* zero;

            berners::Node node;
            berners::PropertyMap specified_values;
            std::vector<berners::StyledNode> children;

            berners::Value* value(std::string name);
            berners::Display display();
            berners::Value* lookup(std::initializer_list<std::string> il);
    };
};

#endif