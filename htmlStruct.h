#ifndef _HTMLSTRUCT_H_
#define _HTMLSTRUCT_H_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace berners {
    typedef std::unordered_map<std::string,std::string> AttrMap;

    enum NodeType {
        Text,
        Element
    };

    class ElementData {
        public:
            std::string tag_name;
            berners::AttrMap attributes;

            ElementData(){}
            ElementData(std::string tag, berners::AttrMap& attributes) : tag_name(tag), attributes(attributes) {}
            bool hasId();
            std::string getId();
            std::unordered_set<std::string> getClasses();
            ~ElementData(){}
    };
    
    struct Node {
        std::vector<berners::Node> children;
        berners::NodeType node_type;
        std::string text;
        berners::ElementData data;
        Node() {}
        Node(std::string data) : children(std::vector<berners::Node>()), node_type(berners::NodeType::Text), text(data) {}
        Node(std::string name, berners::AttrMap& attrs, std::vector<berners::Node>& children) : children(children), node_type(berners::NodeType::Element), data(name,attrs) {}
        ~Node() {}
    };
};

#endif