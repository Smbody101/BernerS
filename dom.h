#ifndef _DOM_H_
#define _DOM_H_
#include <string>
#include <unordered_map>
#include <vector>

namespace berners {

    typedef std::unordered_map<std::string,std::string> AttrMap;

    enum NodeType {
        Text,
        Element
    };

    class ElementData {
        private:
            
        public:
            std::string tag_name;
            berners::AttrMap attributes;

            ElementData();
            ElementData(std::string tag, berners::AttrMap& attributes);
            ~ElementData();
    };

    class Node {
        private:

        public:
            std::vector<berners::Node> children;
            berners::NodeType node_type;
            std::string text;
            berners::ElementData data;

            Node(std::string data);
            Node(std::string name, berners::AttrMap& attrs, std::vector<berners::Node>& children);
            ~Node();
    };

    class Dom {
        private:
            
        public:
            Dom();

            static void printDom(const berners::Node& root, int d);

            ~Dom();
    };
};

#endif
