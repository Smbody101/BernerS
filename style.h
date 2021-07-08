#ifndef _CSS_H_
#define _CSS_H_

#include <vector>
#include "cssStruct.h"
#include "htmlStruct.h"
#include "styleStruct.h"

namespace berners {
    class Style {
        public:
            Style() {}
            ~Style() {}
            bool matches(berners::ElementData& elem, berners::Selector& selector);
            bool matches_simple_selector(berners::ElementData& elem, berners::SimpleSelector& selector);
            berners::MatchedRule match_rule(berners::ElementData& elem, berners::Rule& rule);
            std::vector<berners::MatchedRule> matching_rules(berners::ElementData& elem, berners::Stylesheet& stylesheet);
            berners::PropertyMap specified_values(berners::ElementData& elem, berners::Stylesheet& stylesheet);
            berners::StyledNode style_tree(berners::Node& root, berners::Stylesheet& stylesheet);

            static void printStyleSheet(const berners::Stylesheet& sheet);
            static void printSelector(const berners::Selector& selector);
            static void printStyleTree(const berners::StyledNode& stylenode);
            static void printValue(const berners::Value& value);
    };
};

#endif