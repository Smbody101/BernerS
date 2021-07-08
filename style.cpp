#include <iostream>
#include <algorithm>
#include "style.h"
#include <stdio.h>

berners::Value zeroValue(berners::Length(0.0, berners::Unit::Px));
berners::Value* berners::StyledNode::zero = &zeroValue;

berners::Value* berners::StyledNode::value(std::string name){
    if(this->specified_values.count(name) != 0)
        return &(this->specified_values[name]);
    return nullptr;
}

berners::Value* berners::StyledNode::lookup(std::initializer_list<std::string> il){
    berners::Value* v;
    for (auto ptr = il.begin(); ptr != il.end(); ptr++) {
        v = this->value(*ptr);
		if(v != nullptr){
            return v;
        }
	}
    return berners::StyledNode::zero;
}

berners::Display berners::StyledNode::display(){
    berners::Value* v = this->value("display");
    if(v == nullptr)
        return berners::Display::Inline;
    if(v->key_word == "block")
        return berners::Display::Block;
    else if(v->key_word == "none")
        return berners::Display::None;
    return berners::Display::Inline;
}

bool berners::Style::matches(berners::ElementData& elem, berners::Selector& selector){
    // printf("%s %s %d\n", typeid(selector).name(), typeid(berners::SimpleSelector).name(), typeid(selector) == typeid(berners::SimpleSelector));
    if(typeid(selector) == typeid(berners::SimpleSelector))
        return this->matches_simple_selector(elem, *(dynamic_cast<berners::SimpleSelector*> (&selector)));
    return false;
}

bool berners::Style::matches_simple_selector(berners::ElementData& elem, berners::SimpleSelector& selector){
    if(selector.tag_name != "" && selector.tag_name != elem.tag_name)
        return false;

    if(!((selector.id != "" && elem.hasId() && selector.id == elem.getId()) || selector.id == ""))
        return false;

    if(selector.classes.size() != 0){
        std::unordered_set<std::string> c = elem.getClasses();
        for(std::string s : selector.classes){
            if(c.count(s) != 0)
                return true;
        }
        return false;
    }
    
    return true;
}

berners::MatchedRule berners::Style::match_rule(berners::ElementData& elem, berners::Rule& rule){
    berners::MatchedRule mr;
    mr.valid = false;
    for(berners::Selector* selector : rule.selectors){
        if(this->matches(elem, *selector)){
            mr.rule = rule;
            mr.specificity = selector->specificity();
            mr.valid = true;
            return mr;
        }
    }
    return mr;
}

std::vector<berners::MatchedRule> berners::Style::matching_rules(berners::ElementData& elem, berners::Stylesheet& stylesheet){
    std::vector<berners::MatchedRule> rules;
    for(berners::Rule rule : stylesheet.rules){
        berners::MatchedRule mr = this->match_rule(elem, rule);
        if(mr.valid)
            rules.push_back(mr);
    }
    return rules;
}

berners::PropertyMap berners::Style::specified_values(berners::ElementData& elem, berners::Stylesheet& stylesheet){
    berners::PropertyMap* values = new berners::PropertyMap();
    std::vector<berners::MatchedRule> rules = this->matching_rules(elem, stylesheet);
    std::sort(rules.begin(), rules.end(), [](const berners::MatchedRule& a, const berners::MatchedRule& b){
        return b.specificity < a.specificity;
    });
    for(berners::MatchedRule rule : rules){
        for(berners::Declaration declaration : rule.rule.declarations){
            (*values)[declaration.name] = declaration.value;
        }
    }
    return *values;
}

berners::StyledNode berners::Style::style_tree(berners::Node& root, berners::Stylesheet& stylesheet){
    berners::StyledNode node;
    node.node = root;
    if(root.node_type == berners::NodeType::Element)
        node.specified_values = this->specified_values(root.data, stylesheet);
    for(berners::Node n : root.children)
        node.children.push_back(this->style_tree(n, stylesheet));
    return node;
}

void berners::Style::printSelector(const berners::Selector& selector){
    std::cout << selector.tag_name;
    if(selector.id.length() != 0)
        std::cout << "#" << selector.id;
    
    for(std::string c : selector.classes)
        std::cout << "." << c;
}

void berners::Style::printValue(const berners::Value& value){
    switch(value.type){
        case berners::ValueType::Key: std::cout << value.key_word; break;
        case berners::ValueType::Len:{
            std::cout << value.length.v;
            if(value.length.u == berners::Unit::Px)
                std::cout << "px";
            else if(value.length.u == berners::Unit::Em)
                std::cout << "em";
            else
                std::cout << "unknow";
        };break;
        case berners::ValueType::Col: {
            char trans[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
            std::cout << "#" << trans[value.color.r / 16] << trans[value.color.r % 16];
            std::cout << trans[value.color.g / 16] << trans[value.color.g % 16];
            std::cout << trans[value.color.b / 16] << trans[value.color.b % 16];
        }break;
        default:
            break;
    }
}

void berners::Style::printStyleSheet(const berners::Stylesheet& sheet) {
    for(berners::Rule rule : sheet.rules){
        berners::Style::printSelector(*(rule.selectors[0]));
        for(int i = 1; i < (int)rule.selectors.size(); i++){
            std::cout << ", ";
            berners::Style::printSelector(*(rule.selectors[i]));
        }
        std::cout << " {" << std::endl;
        for(berners::Declaration d : rule.declarations){
            std::cout << "\t" << d.name << ": ";
            berners::Style::printValue(d.value);
            std::cout << ";" << std::endl;
        }
        std::cout << "}" << std::endl;
    }
}

void berners::Style::printStyleTree(const berners::StyledNode& stylenode){
    if(stylenode.node.node_type == berners::NodeType::Text){
        std::cout << stylenode.node.text;
        return;
    }
    std::cout << stylenode.node.data.tag_name << ": {" << std::endl;
    
    if(stylenode.specified_values.size() == 0)
        goto children;
    std::cout << "\tstyle: {" << std::endl;
    for(auto it : stylenode.specified_values){
        std::cout << it.first << ": ";
        berners::Style::printValue(it.second);
        std::cout << ";" << std::endl;
    }
    std::cout << "}," << std::endl;

    children:
    if(stylenode.children.size() == 0)
        goto end;
    std::cout << "\tchildren: [" << std::endl;
    for(int i = 0; i < (int)stylenode.children.size(); i++){
        berners::Style::printStyleTree(stylenode.children[i]);
        if(i < (int)stylenode.children.size() - 1)
            std::cout << ",";
        std::cout << std::endl;
    }
    std::cout << "]" << std::endl;
    end:
    std::cout << "}";
}