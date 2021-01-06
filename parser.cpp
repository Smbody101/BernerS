#include "parser.h"

#include <iostream>

berners::Parser::Parser(const std::string& s) : pos(0U), input(s){

}

char berners::Parser::next_char(){
    return this->input[this->pos];
}

bool berners::Parser::starts_with(const std::string& s){
    unsigned int len = s.length();
    if(this->input.length() - this->pos < len)
        return false;
    for(unsigned int i = 0; i < len; i++){
        if(this->input[this->pos + i] != s[i])
            return false;
    }
    return true;
}

bool berners::Parser::eof(){
    return this->input.length() <= this->pos;
}

char berners::Parser::consume_char(){
    return this->input[this->pos++];
}

std::string berners::Parser::consume_while(std::function<bool(char)> test){
    std::string result = "";
    while(!this->eof() && test(this->next_char())){
        result += this->consume_char();
    }
    return result;
}

void berners::Parser::consume_whitespace(){
    this->consume_while([](char c)->bool { return c == ' ' || c == '\n' || c == '\t';});
}

std::string berners::Parser::parse_tag_name(){
    return this->consume_while([](char c)->bool {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
    });
}

berners::Node berners::Parser::parse_node(){
    if(this->next_char() == '<')
        return this->parse_element();
    else
        return this->parse_text();
}

berners::Node berners::Parser::parse_text(){
    return berners::Node(this->consume_while([](char c)->bool {
            return c != '<';
        })
    );
}

berners::Node berners::Parser::parse_element(){
    if(this->consume_char() != '<'){
        std::cout << "error: not an element!" << std::endl;
        exit(1);
    }

    std::string tag_name = this->parse_tag_name();
    berners::AttrMap attrs = this->parse_attributes();
    if(this->consume_char() != '>'){
        std::cout << "error: not an element!" << std::endl;
        exit(1);
    }

    std::vector<berners::Node> children = this->parse_nodes();

    if(this->consume_char() != '<' || this->consume_char() != '/'){
        std::cout << "error: not an element!" << std::endl;
        exit(1);
    }

    if(this->parse_tag_name().compare(tag_name) != 0){
        std::cout << "error: tag is not matched!" << std::endl;
        exit(1);
    }

    if(this->consume_char() != '>'){
        std::cout << "error: not an element!" << std::endl;
        exit(1);
    }
        
    return berners::Node(tag_name, attrs, children);
}

std::pair<std::string, std::string> berners::Parser::parse_attr(){
    std::string name = this->parse_tag_name();
    if(this->consume_char() != '='){
        std::cout << "error: not an attribute!" << std::endl;
        exit(1);
    }
    std::string value = this->parse_attr_value();
    return std::pair<std::string, std::string>(name, value);
}

std::string berners::Parser::parse_attr_value(){
    char open_quote = this->consume_char();
    if(open_quote != '"' && open_quote != '\''){
        std::cout << "error: not an attribute!" << std::endl;
        exit(1);
    }

    std::string value = this->consume_while([open_quote](char c)->bool {
        return c != open_quote;
    });

    if(this->consume_char() != open_quote){
        std::cout << "error: not matched quote!" << std::endl;
        exit(1);
    }
    return value; 
}

berners::AttrMap berners::Parser::parse_attributes(){
    berners::AttrMap attributes;
    for(;;){
        this->consume_whitespace();
        if(this->next_char() == '>')
            break;
        std::pair<std::string, std::string> p = this->parse_attr();
        attributes[p.first] = p.second;
    }
    return attributes;
}

std::vector<berners::Node> berners::Parser::parse_nodes(){
    std::vector<berners::Node> nodes;
    for(;;){
        this->consume_whitespace();
        if(this->eof() || this->starts_with("</"))
            break;
        nodes.push_back(this->parse_node());
    }
    return nodes;
}

berners::Node berners::Parser::parse(const std::string& source){
    std::vector<berners::Node> nodes = Parser(source).parse_nodes();

    if(nodes.size() == 1)
        return nodes[0];

    berners::AttrMap attr;
    berners::Node root("html", attr, nodes);
    return root;
}

berners::Parser::~Parser(){

}