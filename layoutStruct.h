#ifndef _LAYOUTSTRUCT_H_
#define _LAYOUTSTRUCT_H_

#include <vector>
#include "styleStruct.h"

namespace berners{

    struct EdgeSizes{
        float left;
        float right;
        float top;
        float bottom;

        EdgeSizes() : left(0.0), right(0.0), top(0.0), bottom(0.0) {}
    };

    struct Rect{
        float x;
        float y;
        float width;
        float height;

        Rect() : x(0.0), y(0.0), width(0.0), height(0.0) {}
        Rect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

        berners::Rect expanded_by(berners::EdgeSizes& edge){
            return berners::Rect(this->x - edge.left, this->y - edge.top, this->width + edge.left + edge.right, this->height + edge.top + edge.bottom);
        }
    };

    struct Dimensions{
        berners::Rect content;
        berners::EdgeSizes padding;
        berners::EdgeSizes border;
        berners::EdgeSizes margin;

        Dimensions() {}
        Dimensions(berners::Rect content) : content(content), padding(), border(), margin() {}

        berners::Rect padding_box() {
            return this->content.expanded_by(this->padding);
        }

        berners::Rect border_box() {
            return this->padding_box().expanded_by(this->border);
        }

        berners::Rect margin_box() {
            return this->border_box().expanded_by(this->margin);
        }
    };

    enum BoxType{
        BlockNode, InlineNode, AnonymousBlock
    };

    class LayoutBox{
        private:
            void layout_block(berners::Dimensions& containing_block);
            void calculate_block_width(berners::Dimensions& containing_block);
            void calculate_block_position(berners::Dimensions& containing_block);
            void layout_block_children();
            void calculate_block_height();

        public:
            berners::Dimensions dimensions;
            berners::BoxType box_type;
            berners::StyledNode node;
            std::vector<berners::LayoutBox> children;

            berners::LayoutBox get_inline_container();
            void layout(berners::Dimensions& containing_block);
    };
};

#endif