#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include "layoutStruct.h"
#include "styleStruct.h"

namespace berners {
    class Layout {
        public:
            static berners::LayoutBox layout_tree(berners::StyledNode& node, berners::Dimensions containing_block);
            static berners::LayoutBox build_layout_tree(berners::StyledNode& style_node);
            static void print_layout_tree(berners::LayoutBox& layout_box);
    };
};

#endif