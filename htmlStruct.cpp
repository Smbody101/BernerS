#include "htmlStruct.h"
#include "util.h"

bool berners::ElementData::hasId(){
    return this->attributes.count("id") != 0;
}

std::string berners::ElementData::getId(){
    if(!this->hasId())
        return "";
    return this->attributes["id"];
}

std::unordered_set<std::string> berners::ElementData::getClasses(){
    std::unordered_set<std::string> c;
    if(this->attributes.count("class") == 0)
        return c;
    std::vector<std::string> classes = berners::Util::split(this->attributes["class"], ' ');
    for(std::string s : classes)
        c.insert(s);
    return c;
}