#include <iostream>
#include "dom.h"

berners::ElementData::ElementData() : tag_name(""), attributes(berners::AttrMap()) {

}

berners::ElementData::ElementData(std::string tag, berners::AttrMap& attributes) : tag_name(tag), attributes(attributes) {

}

berners::ElementData::~ElementData() {

}

berners::Node::Node(std::string data) : children(std::vector<berners::Node>()), node_type(berners::NodeType::Text), text(data) {

}
berners::Node::Node(std::string name, berners::AttrMap& attrs, std::vector<berners::Node>& children) : children(children), node_type(berners::NodeType::Element), data(name,attrs) {

}

berners::Node::~Node() {

}

berners::Dom::Dom()
{
    
}

void berners::Dom::printDom(const berners::Node& root, int d) {

    for(int i = 0; i < d; i++)
        std::cout << "  ";

    if(root.node_type == berners::NodeType::Text) {
        std::cout << root.text << std::endl;
        return;
    }

    std::cout << "<" << root.data.tag_name;
    for(auto i = root.data.attributes.begin(); i != root.data.attributes.end(); i++) {
        std::cout << " " << i->first << "=\"" << i->second << "\"";
    }
    std::cout << ">" << std::endl;

    for(auto i = root.children.begin(); i != root.children.end(); i++)
        printDom(*i, d + 1);

    for(int i = 0; i < d; i++)
        std::cout << "  ";
    std::cout << "<" << root.data.tag_name << "/>" << std::endl;
}

berners::Dom::~Dom() {

}