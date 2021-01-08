#ifndef _CSS_H_
#define _CSS_H_
#include <vector>
#include <string>

namespace berners {
    enum Unit {
        Px, Em
    };

    enum ValueType {
        Key, Len, Col
    };

    class Color {
        public:
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;

            Color(std::string color);
            ~Color();
    };

    class Length {
        public:
            double v;
            berners::Unit u;

            Length(double v, std::string u);
            ~Length();
    };

    class Value {
        public:
            std::string key_word;
            berners::Length length;
            berners::Color color;
            berners::ValueType type;

            Value(std::string key_word);
            Value(berners::Length& length);
            Value(berners::Color& color);
            ~Value();
    };

    class Declaration {
        public:
            std::string name;
            berners::Value value;

            Declaration(std::string name, berners::Value& value);
            ~Declaration();
    };

    class Selector {
        public:
            std::string tag_name;
            std::string id;
            std::vector<std::string> classes;
    };

    class SimpleSelector : public Selector {
        public:
            SimpleSelector(std::string name, std::string id, std::vector<std::string>& classes);
            ~SimpleSelector();
    };

    class Rule {
        public:
            std::vector<berners::Selector> selectors;
            std::vector<berners::Declaration> declarations;

            Rule(std::vector<berners::Selector>& selectors, std::vector<berners::Declaration>& declarations);
            ~Rule();
    };

    class Stylesheet {
        private:

        public:
            std::vector<berners::Rule> rules;

            Stylesheet(std::vector<berners::Rule>& rules);
            ~Stylesheet();
    };
};

#endif