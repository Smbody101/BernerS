#ifndef _RENDER_H_
#define _RENDER_H_

#include <vector>
#include <string>
#include "cssStruct.h"
#include "layoutStruct.h"

namespace berners {

    enum DisplayCommandType {
        SolidColor
    };

    struct DisplayCommand {
        berners::DisplayCommandType type;
        berners::Color color;
        berners::Rect rect;

        DisplayCommand() {}
        DisplayCommand(berners::DisplayCommandType type, berners::Color color, berners::Rect rect) : type(type), color(color), rect(rect) {
            
        }
    };

    typedef std::vector<berners::DisplayCommand> DisplayList;

    class Canvas {

        private:
            uint32_t width;
            uint32_t height;

        public:
            std::vector<berners::Color> pixels;
            Canvas(uint32_t width, uint32_t height);
            void paint_item(berners::DisplayCommand& item);
            void print_picture();
    };

    class Render {
        public:
            static berners::DisplayList build_display_list(berners::LayoutBox& layout_root);
            static void render_layout_box(berners::DisplayList& list, berners::LayoutBox& layout_box);
            static void render_background(berners::DisplayList& list, berners::LayoutBox& layout_box);
            static berners::Color* get_color(berners::LayoutBox& layout_box, std::string name);
            static void render_borders(berners::DisplayList& list, berners::LayoutBox& layout_box);
            static berners::Canvas paint(berners::LayoutBox& layout_root, berners::Rect bounds);
    };
};

#endif