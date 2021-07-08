#include "parser.h"

#include <iostream>
#include <algorithm>
/**
 * 
 * Parser implementation
 * 
 **/
char berners::Parser::next_char() {
    return this->input[this->pos];
}

bool berners::Parser::starts_with(const std::string& s) {
    unsigned int len = s.length();
    if(this->input.length() - this->pos < len)
        return false;
    for(unsigned int i = 0; i < len; i++) {
        if(this->input[this->pos + i] != s[i])
            return false;
    }
    return true;
}

bool berners::Parser::eof() {
    return this->input.length() <= this->pos;
}

char berners::Parser::consume_char() {
    return this->input[this->pos++];
}

std::string berners::Parser::consume_while(std::function<bool(char)> test) {
    std::string result = "";
    while(!this->eof() && test(this->next_char())) {
        result += this->consume_char();
    }
    return result;
}

void berners::Parser::consume_whitespace() {
    this->consume_while([](char c)->bool { return c == ' ' || c == '\n' || c == '\t';});
}

/**
 * 
 * HtmlParser implementation
 * 
 **/

std::string berners::HtmlParser::parse_tag_name() {
    return this->consume_while([](char c)->bool {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
    });
}

berners::Node berners::HtmlParser::parse_node() {
    if(this->next_char() == '<')
        return this->parse_element();
    else
        return this->parse_text();
}

berners::Node berners::HtmlParser::parse_text() {
    return berners::Node(this->consume_while([](char c)->bool {
            return c != '<';
        })
    );
}

berners::Node berners::HtmlParser::parse_element() {
    if(this->consume_char() != '<') {
        std::cout << "error: not an element!" << std::endl;
        exit(1);
    }

    std::string tag_name = this->parse_tag_name();
    berners::AttrMap attrs = this->parse_attributes();
    if(this->consume_char() != '>') {
        std::cout << "error: not an element!" << std::endl;
        exit(1);
    }

    std::vector<berners::Node> children = this->parse_nodes();

    if(this->consume_char() != '<' || this->consume_char() != '/') {
        std::cout << "error: not an element!" << std::endl;
        exit(1);
    }

    if(this->parse_tag_name().compare(tag_name) != 0) {
        std::cout << "error: tag is not matched!" << std::endl;
        exit(1);
    }

    if(this->consume_char() != '>') {
        std::cout << "error: not an element!" << std::endl;
        exit(1);
    }
        
    return berners::Node(tag_name, attrs, children);
}

std::pair<std::string, std::string> berners::HtmlParser::parse_attr() {
    std::string name = this->parse_tag_name();
    if(this->consume_char() != '=') {
        std::cout << "error: not an attribute!" << std::endl;
        exit(1);
    }
    std::string value = this->parse_attr_value();
    return std::pair<std::string, std::string>(name, value);
}

std::string berners::HtmlParser::parse_attr_value() {
    char open_quote = this->consume_char();
    if(open_quote != '"' && open_quote != '\'') {
        std::cout << "error: not an attribute!" << std::endl;
        exit(1);
    }

    std::string value = this->consume_while([open_quote](char c)->bool {
        return c != open_quote;
    });

    if(this->consume_char() != open_quote) {
        std::cout << "error: not matched quote!" << std::endl;
        exit(1);
    }
    return value; 
}

berners::AttrMap berners::HtmlParser::parse_attributes() {
    berners::AttrMap attributes;
    for(;;) {
        this->consume_whitespace();
        if(this->next_char() == '>')
            break;
        std::pair<std::string, std::string> p = this->parse_attr();
        attributes[p.first] = p.second;
    }
    return attributes;
}

std::vector<berners::Node> berners::HtmlParser::parse_nodes() {
    std::vector<berners::Node> nodes;
    for(;;) {
        this->consume_whitespace();
        if(this->eof() || this->starts_with("</"))
            break;
        nodes.push_back(this->parse_node());
    }
    return nodes;
}

berners::Node berners::HtmlParser::parse(const std::string& source) {
    std::vector<berners::Node> nodes = HtmlParser(source).parse_nodes();

    if(nodes.size() == 1)
        return nodes[0];

    berners::AttrMap attr;
    berners::Node root("html", attr, nodes);
    return root;
}

/**
 * 
 * CssParser implementation
 * 
 **/
bool berners::CssParser::valid_identifier_char(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

bool berners::CssParser::valid_key_char(char c){
    return (c >= 'a' && c <= 'z') || c == '-';
}

std::string berners::CssParser::parse_identifier() {
    return this->consume_while([](char c)->bool {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
    });
}

std::string berners::CssParser::parse_name(){
    return this->consume_while([](char c)->bool {
        return (c >= 'a' && c <= 'z') || c == '-';
    });
}

std::string berners::CssParser::parse_keyword(){
    return this->consume_while([](char c)->bool {
        return c >= 'a' && c <= 'z';
    });
}

berners::Length berners::CssParser::parse_length(){
    std::string v_string = this->consume_while([](char c)->bool {
        return (c >= '0' && c <= '9') || c == '.';
    });
    this->consume_whitespace();
    std::string u_string;
    u_string += this->consume_char();
    u_string += this->consume_char();
    berners::Length length(v_string, u_string);
    return length;
}

berners::Color berners::CssParser::parse_color() {
    std::string color_string = this->consume_while([](char c)->bool {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
    });
    berners::Color color(color_string);
    return color;
}

berners::SimpleSelector* berners::CssParser::parse_simple_selector() {
    berners::SimpleSelector* selector = new berners::SimpleSelector();
    while(!this->eof()){
        switch (this->next_char())
        {
        case '#':{
            this->consume_char();
            selector->id = this->parse_identifier();
        }break;
        case '.':{
            this->consume_char();
            selector->classes.push_back(this->parse_identifier());
        }break;
        case '*':{
            this->consume_char();
        }break;
        default:{
            if(!this->valid_identifier_char(this->next_char()))
                return selector;
            selector->tag_name = this->parse_identifier();
        }break;
        }
    }
    return selector;
}

berners::Declaration berners::CssParser::parse_declaration() {
    berners::Declaration declaration;
    while(!this->eof()){
        this->consume_whitespace();
        switch (this->next_char())
        {
        case ':':{
            this->consume_char();
            this->consume_whitespace();
            declaration.value = this->parse_value();
        }break;
        case ';':return declaration;
        default:{
            if(!this->valid_key_char(this->next_char()))
                return declaration;
            declaration.name = this->parse_name();
        }break;
        }
    }
    return declaration;
}

berners::Value berners::CssParser::parse_value(){
    berners::Value value;
    switch (this->next_char())
    {
    case '#':{
        this->consume_char();
        value.type = berners::ValueType::Col;
        value.color = this->parse_color();
    }break;
    case 'a'...'z':{
        value.type = berners::ValueType::Key;
        value.key_word = this->parse_keyword();
    }break;
    case '0'...'9':
    case '.':{
        value.type = berners::ValueType::Len;
        value.length = this->parse_length();
    }break;
    default:
        break;
    }
    return value;
}

berners::Rule berners::CssParser::parse_rule(){
    berners::Rule rule;

    for(;;){
        this->consume_whitespace();
        if(this->next_char() == '{'){
            this->consume_char();
            break;
        }
        else if(this->next_char() == ','){
            this->consume_char();
            this->consume_whitespace();
        }else
            rule.selectors.push_back(this->parse_simple_selector());
    }


    for(;;){
        this->consume_whitespace();
        if(this->next_char() == '}'){
            this->consume_char();
            break;
        }   
        else if(this->next_char() == ';'){
            this->consume_char();
            this->consume_whitespace();
        }else
            rule.declarations.push_back(this->parse_declaration());
    }

    std::sort(rule.selectors.begin(), rule.selectors.end());
    return rule;
}

std::vector<berners::Rule> berners::CssParser::parse_rules() {
    std::vector<berners::Rule> rules;
    for(;;) {
        this->consume_whitespace();
        if(this->eof())
            break;
        rules.push_back(this->parse_rule());
    }
    return rules;
}

berners::Stylesheet berners::CssParser::parse(const std::string& source){
    berners::Stylesheet sheet;
    sheet.rules = berners::CssParser(source).parse_rules();
    return sheet;
}