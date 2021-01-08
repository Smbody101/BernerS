#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <functional>
#include "dom.h"
#include "css.h"

namespace berners {
    class Parser {
        private:
            unsigned int pos;
            std::string input;
        public:
            Parser(const std::string& s);
            char next_char();
            bool starts_with(const std::string& s);
            bool eof();
            char consume_char();
            std::string consume_while(std::function<bool(char)> test);
            void consume_whitespace();
            std::string parse_tag_name();
            berners::Node parse_node();
            berners::Node parse_text();
            berners::Node parse_element();
            std::pair<std::string, std::string> parse_attr();
            std::string parse_attr_value();
            berners::AttrMap parse_attributes();
            std::vector<berners::Node> parse_nodes();
            static berners::Node parse(const std::string& source);
            ~Parser();
    };

    class CssParser : public Parser {
    public:
        CssParser(const std::string& s);
        std::string parse_identifier();
        berners::SimpleSelector parse_simple_selector();
        ~CssParser();
        
    };
};

#endif
