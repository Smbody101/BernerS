#include "util.h"

std::vector<std::string> berners::Util::split(const std::string& s, char separator){
    std::vector<std::string> ans;
    int start = 0;
    int end;
    while((end = s.find_first_of(separator,start)) != (int)std::string::npos){
        ans.push_back(s.substr(start, end - start));
        start = end + 1;
    }
    if(start < (int)s.length())
        ans.push_back(s.substr(start,s.length() - start));
    return ans;
}