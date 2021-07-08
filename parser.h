#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <functional>
#include "htmlStruct.h"
#include "cssStruct.h"

namespace berners {
    class Parser {
        private:
            unsigned int pos;
            std::string input;
        protected:
            Parser(const std::string& s) : pos(0U), input(s){}
            ~Parser(){}

            char next_char();
            bool starts_with(const std::string& s);
            bool eof();
            char consume_char();
            std::string consume_while(std::function<bool(char)> test);
            void consume_whitespace();

    };

    class HtmlParser : Parser {
        public:
            HtmlParser(const std::string& s) : Parser(s){}
            ~HtmlParser(){}

            std::string parse_tag_name();
            berners::Node parse_node();
            berners::Node parse_text();
            berners::Node parse_element();
            std::pair<std::string, std::string> parse_attr();
            std::string parse_attr_value();
            berners::AttrMap parse_attributes();
            std::vector<berners::Node> parse_nodes();
            static berners::Node parse(const std::string& source);
    };

    class CssParser : Parser {
        private:
            bool valid_identifier_char(char c);
            bool valid_key_char(char c);

        public:
            CssParser(const std::string& s) : Parser(s){}
            ~CssParser(){}

            std::string parse_identifier();
            std::string parse_name();
            std::string parse_keyword();
            berners::Length parse_length();
            berners::Color parse_color();
            berners::Value parse_value();
            berners::SimpleSelector* parse_simple_selector();
            berners::Declaration parse_declaration();
            berners::Rule parse_rule();
            std::vector<berners::Rule> parse_rules();
            static berners::Stylesheet parse(const std::string& source);
    };
};

#endif
