#ifndef _CSSSTRUCT_H_
#define _CSSSTRUCT_H_

#include <string>
#include <vector>
#include <unordered_map>

namespace berners {
    enum Unit {
        Px, Em, Invalid
    };

    enum ValueType {
        Key, Len, Col
    };

    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        Color() {}
        Color(std::string color) {
            std::unordered_map<char, int> m;
            for(char c = '0'; c <= '9'; c++)
                m[c] = c - '0';
            for(char c = 'a'; c <= 'f'; c++)
                m[c] = c - 'a' + 10;

            a = 0;
            r = m[color[0]] * 16 + m[color[1]];
            g = m[color[2]] * 16 + m[color[3]];
            b = m[color[4]] * 16 + m[color[5]];
        }
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
        // Color(const berners::Color& color){
        //     r = color.r;
        //     g = color.g;
        //     b = color.b;
        //     a = color.a;
        // }
        std::string toString(){
            char hash[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
            std::string ans;
            ans.push_back(hash[r / 16]);
            ans.push_back(hash[r % 16]);
            ans.push_back(hash[g / 16]);
            ans.push_back(hash[g % 16]);
            ans.push_back(hash[b / 16]);
            ans.push_back(hash[b % 16]);
            return ans;
        }
        ~Color() {}
    };
    
    struct Length {
        double v;
        berners::Unit u;
        bool isAuto;
        
        berners::Length& operator=(const berners::Length& length){
            v = length.v;
            u = length.u;
            isAuto = length.isAuto;
            return *this;
        }

        Length() : isAuto(false) {}
        Length(double v, berners::Unit u) : v(v), u(u), isAuto(false) {}
        Length(std::string v_string, std::string u) : isAuto(false) {
            v = 0;
            bool isInteger = true;
            double e = 0.1;
            for(char c : v_string){
                if(c == '.')
                    isInteger = false;
                else if(isInteger){
                    v = v * 10 + c - '0';
                }else{
                    v += e * (c - '0');
                    e /= 10;
                }
            }

            if(u == "px")
                this->u = berners::Unit::Px;
            else if(u == "em")
                this->u = berners::Unit::Em;
            else
                this->u = berners::Unit::Invalid;
        }
        ~Length() {}
    };



    struct Value {
        berners::ValueType type;
        std::string key_word;
        berners::Length length;
        berners::Color color;

        berners::Value& operator=(const berners::Value& value){
            type = value.type;
            switch (type) {
            case ValueType::Key : key_word = value.key_word;break;
            case ValueType::Len : length = value.length;break;
            case ValueType::Col : color = value.color;break;
            
            default:
                break;
            }
            return *this;
        }

        Value() {}
        Value(const berners::Value& value){
            type = value.type;
            switch (type) {
            case ValueType::Key : key_word = value.key_word;break;
            case ValueType::Len : length = value.length;break;
            case ValueType::Col : color = value.color;break;
            
            default:
                break;
            }
        }
        Value(std::string key_word) : type(ValueType::Key), key_word(key_word) {}
        Value(const berners::Length& length) : type(ValueType::Len), length(length) {}
        Value(berners::Color& color) : type(ValueType::Col), color(color) {}
        ~Value() {}

        double to_px() const {
            if(type != berners::ValueType::Len)
                return 0;
            if(length.isAuto)
                return 0;
            return length.v;
        }
        
    };

    struct Declaration {
        std::string name;
        berners::Value value;

        Declaration() {}
        Declaration(const berners::Declaration& declaration) {
            name = declaration.name;
            value = declaration.value;
        }

        Declaration(std::string name, berners::Value& value) : name(name) {
            this->value = value;
        }

        ~Declaration() {}
    };

    struct Specificity {
        int a;
        int b;
        int c;
        Specificity() {}
        Specificity(int a, int b, int c) : a(a), b(b), c(c){}
        bool operator<(const Specificity& other) const{
            if(this->a != other.a)
                return this->a < other.a;
            if(this->b != other.b)
                return this->b < other.b;
            return this->c < other.c;
        }
        ~Specificity() {}
    };

    class Selector {
        private:
            int a() const {
                if(id.length() == 0)
                    return 0;
                return 1;
            }

            int b() const {
                return classes.size();
            }

            int c() const {
                if(tag_name.length() == 0)
                    return 0;
                return 1;
            }

        public:
            std::string tag_name;
            std::string id;
            std::vector<std::string> classes;

            Selector() {
                tag_name = "";
                id = "";
            }
            Selector(std::string name, std::string id, std::vector<std::string>& classes) : tag_name(name), id(id), classes(classes) {}
            berners::Specificity specificity() const{
                return berners::Specificity(this->a(), this->b(), this->c());
            }


            bool operator<(const berners::Selector& other){
                return this->specificity() < (other.specificity());
            }
            
            virtual ~Selector() {}
    };

    class SimpleSelector : public Selector {
        public:
            SimpleSelector() : Selector() {}
            SimpleSelector(std::string name, std::string id, std::vector<std::string>& classes) : Selector(name, id, classes) {}
            ~SimpleSelector() {}
    };
    
    struct Rule {
        std::vector<berners::Selector*> selectors;
        std::vector<berners::Declaration> declarations;
    };

    struct Stylesheet {
        std::vector<berners::Rule> rules;

        Stylesheet() {}
        ~Stylesheet() {}
    };
}

#endif