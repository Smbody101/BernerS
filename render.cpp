#include "render.h"

berners::DisplayList berners::Render::build_display_list(berners::LayoutBox& layout_root){
    berners::DisplayList list;
    berners::Render::render_layout_box(list, layout_root);
    return list;
}

void berners::Render::render_layout_box(berners::DisplayList& list, berners::LayoutBox& layout_box){
    berners::Render::render_background(list, layout_box);
    berners::Render::render_borders(list, layout_box);
    
    for(berners::LayoutBox child : layout_box.children)
        berners::Render::render_layout_box(list, child);
}

void berners::Render::render_background(berners::DisplayList& list, berners::LayoutBox& layout_box){
    berners::Color* color = berners::Render::get_color(layout_box, "background");
    if(color != nullptr)
        list.push_back(berners::DisplayCommand(berners::DisplayCommandType::SolidColor, *color, layout_box.dimensions.border_box()));
}

berners::Color* berners::Render::get_color(berners::LayoutBox& layout_box, std::string name){
    
    switch (layout_box.box_type)
    {
    case berners::BoxType::BlockNode:
    case berners::BoxType::InlineNode: {
        berners::Value* v = layout_box.node.value(name);
        if(v != nullptr)
        return &v->color;
        else
            return nullptr;
    }break;
    default:
        return nullptr;
    }
}

void berners::Render::render_borders(berners::DisplayList& list, berners::LayoutBox& layout_box){
    berners::Color* color = berners::Render::get_color(layout_box, "border-color");
    if(color == nullptr)
        return;
    berners::Dimensions* d = &layout_box.dimensions;
    berners::Rect border_box = d->border_box();

    list.push_back(berners::DisplayCommand(berners::DisplayCommandType::SolidColor, 
        *color, berners::Rect(border_box.x, border_box.y, d->border.left, border_box.height)
    ));

    list.push_back(berners::DisplayCommand(berners::DisplayCommandType::SolidColor, 
        *color, berners::Rect(border_box.x + border_box.width - d->border.right, border_box.y, d->border.right, border_box.height)
    ));

    list.push_back(berners::DisplayCommand(berners::DisplayCommandType::SolidColor, 
        *color, berners::Rect(border_box.x, border_box.y, border_box.width, d->border.top)
    ));

    list.push_back(berners::DisplayCommand(berners::DisplayCommandType::SolidColor, 
        *color, berners::Rect(border_box.x, border_box.y + border_box.height - d->border.bottom, border_box.width, d->border.bottom)
    ));
}

berners::Canvas berners::Render::paint(berners::LayoutBox& layout_root, berners::Rect bounds){
    berners::DisplayList display_list = berners::Render::build_display_list(layout_root);
    berners::Canvas canvas(bounds.width, bounds.height);
    for(berners::DisplayCommand item : display_list){
        canvas.paint_item(item);
    }
    return canvas;
}

berners::Canvas::Canvas(uint32_t width, uint32_t height) : width(width), height(height){
    for(uint32_t i = 0; i < width * height; i++)
        pixels.push_back(berners::Color(255,255,255,255));
}

void berners::Canvas::paint_item(berners::DisplayCommand& item){
    switch (item.type)
    {
    case berners::DisplayCommandType::SolidColor:{
        int x0 = item.rect.x < 0.0 ? 0 : (item.rect.x > this->width - 1 ? this->width - 1 : (uint32_t)item.rect.x);
        int y0 = item.rect.y < 0.0 ? 0 : (item.rect.y > this->height - 1 ? this->height - 1 : (uint32_t)item.rect.y);

        float xt = item.rect.x + item.rect.width;
        float yt = item.rect.y + item.rect.height;

        int x1 = xt < 0.0 ? 0 : (xt > this->width - 1 ? this->width - 1: (uint32_t)xt);
        int y1 = yt < 0.0 ? 0 : (yt > this->height - 1 ? this->height - 1: (uint32_t)yt);

        for(int y = y0; y <= y1; y++){
            int base = y * this->width;
            for(int x = x0; x <= x1; x++)
                this->pixels[x + base] = item.color;
        }
    }break;
    
    default:
        break;
    }
}

void berners::Canvas::print_picture(){
    for(uint32_t i = 0; i < this->height; i++){
        std::cout << this->pixels[i * this->width].toString();
        for(uint32_t j = 1; j < this->width; j++)
            std::cout << "," << this->pixels[i * this->width + j].toString();
        std::cout << std::endl;
    }
}