#include <iostream>
#include "layout.h"

#define LENISNOTAUTO(x) (x->type == berners::ValueType::Len && !x->length.isAuto)

berners::LayoutBox berners::LayoutBox::get_inline_container(){
    switch (this->box_type)
    {
    case berners::BoxType::InlineNode:
    case berners::BoxType::AnonymousBlock: return *this;
    default:{
        if(children.empty() || children[children.size() - 1].box_type != berners::BoxType::AnonymousBlock){
            berners::LayoutBox b;
            b.box_type = berners::BoxType::AnonymousBlock;
            children.push_back(b);
        }
        return children[children.size() - 1];
    }break;
    }
}

void berners::LayoutBox::layout(berners::Dimensions& containing_block){
    switch (this->box_type)
    {
    case berners::BoxType::BlockNode : this->layout_block(containing_block);break;
    
    default:
        break;
    }
}

void berners::LayoutBox::layout_block(berners::Dimensions& containing_block){
    this->calculate_block_width(containing_block);
    this->calculate_block_position(containing_block);
    this->layout_block_children();
    this->calculate_block_height();
}

void berners::LayoutBox::calculate_block_width(berners::Dimensions& containing_block){
    berners::StyledNode style = this->node;
    berners::Value* width = style.value("width");
    if(width == nullptr)
        width = new berners::Value("auto");
    
    const berners::Value* margin_left = style.lookup({"margin-left", "margin"});
    const berners::Value* margin_right = style.lookup({"margin-right", "margin"});
    const berners::Value* border_left = style.lookup({"border-left-width", "border-width"});
    const berners::Value* border_right = style.lookup({"margin-left", "border-width"});
    const berners::Value* padding_left = style.lookup({"padding-left", "padding"});
    const berners::Value* padding_right = style.lookup({"padding-right", "padding"});

    double total = margin_left->to_px() + margin_right->to_px() + border_left->to_px()
                + border_right->to_px() + padding_left->to_px() + padding_right->to_px() + width->to_px();

    if(LENISNOTAUTO(width) && total > containing_block.content.width){
        if(!LENISNOTAUTO(margin_left))
            margin_left = berners::StyledNode::zero;
        if(!LENISNOTAUTO(margin_right))
            margin_right = berners::StyledNode::zero;
    }
    
    double underflow = containing_block.content.width - total;

    if(LENISNOTAUTO(width) && LENISNOTAUTO(margin_left) && LENISNOTAUTO(margin_right))
        margin_right = new berners::Value(Length(margin_right->to_px() + underflow, berners::Unit::Px));
    else if(LENISNOTAUTO(width) && LENISNOTAUTO(margin_left) && !LENISNOTAUTO(margin_right))
        margin_right = new berners::Value(Length(underflow, berners::Unit::Px));
    else if(LENISNOTAUTO(width) && !LENISNOTAUTO(margin_left) && LENISNOTAUTO(margin_right))
        margin_left = new berners::Value(Length(underflow, berners::Unit::Px));
    else if(!LENISNOTAUTO(width)){
            if(!LENISNOTAUTO(margin_left)) margin_left = berners::StyledNode::zero;
            if(!LENISNOTAUTO(margin_right)) margin_right = berners::StyledNode::zero;

            if(underflow >= 0.0)
                width = new berners::Value(Length(underflow, berners::Unit::Px));
            else{
                width = berners::StyledNode::zero;
                margin_right = new berners::Value(Length(margin_right->to_px() + underflow, berners::Unit::Px));
            }
    }else if(LENISNOTAUTO(width) && !LENISNOTAUTO(margin_left) && !LENISNOTAUTO(margin_right))
        margin_left = margin_right = new berners::Value(Length(underflow / 2, berners::Unit::Px));
    
    berners::Dimensions* d = &this->dimensions;

    d->content.width = width->to_px();

    d->padding.left = padding_left->to_px();
    d->padding.right = padding_right->to_px();

    d->border.left = border_left->to_px();
    d->border.right = border_right->to_px();

    d->margin.left = margin_left->to_px();
    d->margin.right = margin_right->to_px();
}

void berners::LayoutBox::calculate_block_position(berners::Dimensions& containing_block){
    berners::StyledNode style = this->node;
    berners::Dimensions* d = &this->dimensions;

    d->margin.top = style.lookup({"margin-top", "margin"})->to_px();
    d->margin.bottom = style.lookup({"margin-bottom", "margin"})->to_px();

    d->border.top = style.lookup({"border-top-width", "border-width"})->to_px();
    d->border.bottom = style.lookup({"border-bottom-width", "border-width"})->to_px();

    d->padding.top = style.lookup({"padding-top", "padding"})->to_px();
    d->padding.bottom = style.lookup({"padding-bottom", "padding"})->to_px();

    d->content.x = containing_block.content.x + d->margin.left + d->border.left + d->padding.left;
    d->content.y = containing_block.content.height + containing_block.content.y + d->margin.top + d->border.top + d->padding.top;
}

void berners::LayoutBox::layout_block_children(){
    for(berners::LayoutBox& child : this->children){
        child.layout(this->dimensions);
        this->dimensions.content.height += child.dimensions.margin_box().height;
    }
}

void berners::LayoutBox::calculate_block_height(){
    berners::Value* v = this->node.value("height");
    if(v != nullptr && LENISNOTAUTO(v))
        this->dimensions.content.height = v->length.v;
}

berners::LayoutBox berners::Layout::build_layout_tree(berners::StyledNode& style_node){
    berners::LayoutBox root;
    switch (style_node.display())
    {
    case berners::Display::Block: root.box_type = berners::BoxType::BlockNode;root.node = style_node;break;
    case berners::Display::Inline: root.box_type = berners::BoxType::InlineNode;root.node = style_node;break;
    default:std::cout << "Root node has display: none." << std::endl;exit(1);break;
    }

    for(berners::StyledNode child : style_node.children){
        switch (child.display())
        {
        case berners::Display::Block: root.children.push_back(berners::Layout::build_layout_tree(child));break;
        case berners::Display::Inline: root.get_inline_container().children.push_back(berners::Layout::build_layout_tree(child));break;
        default:break;
        }
    }

    return root;
}

void berners::Layout::print_layout_tree(berners::LayoutBox& layout_box){
    std::cout << "[" << layout_box.dimensions.content.height << "," << layout_box.dimensions.content.width << "]";
    switch (layout_box.box_type)
    {
    case berners::BoxType::BlockNode: std::cout << "Block";break;
    case berners::BoxType::InlineNode: std::cout << "Inline";break;
    default: std::cout << "Anonymous";break;
    }
    std::cout << ":{" << std::endl;
    for(int i = 0; i < (int)layout_box.children.size(); i++){
        print_layout_tree(layout_box.children[i]);
        if(i < (int)layout_box.children.size() - 1)
            std::cout << "," << std::endl;
    }
    std::cout << "}" << std::endl;
}

berners::LayoutBox berners::Layout::layout_tree(berners::StyledNode& node, berners::Dimensions containing_block){
    containing_block.content.height = 0.0;
    LayoutBox root_box = Layout::build_layout_tree(node);
    root_box.layout(containing_block);
    return root_box;
}