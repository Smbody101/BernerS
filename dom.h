#include <string>
#include <map>
#include <vector>

namespace berners{

    typedef std::map<std::string,std::string> AttrMap;

    enum NodeType{
        Text,
        Element
    };

    struct ElementData{
        std::string tag_name;
        berners::AttrMap attributes;

        ElementData() : tag_name(""), attributes(berners::AttrMap()){}
        ElementData(std::string tag, berners::AttrMap& attributes) : tag_name(tag), attributes(attributes){}

        ~ElementData(){}
    };

    struct Node{
        std::vector<berners::Node> children;
        berners::NodeType node_type;
        std::string text;
        berners::ElementData data;
        
        Node(std::string data) : children(std::vector<berners::Node>()), node_type(berners::NodeType::Text), text(data){}
        Node(std::string name, berners::AttrMap& attrs, std::vector<berners::Node>& children) : children(children), node_type(berners::NodeType::Element), data(name,attrs){}

        ~Node(){}
    };

    class Dom
    {
        private:
            
        public:
            Dom();

            static void printDom(berners::Node& root, int d);

            ~Dom();
    };
};