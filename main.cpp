#include "dom.h"
#include "parser.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace berners;

int main(){
    ifstream readFile("test.html");
    char temp[1024];
    string source = "";
    while(!readFile.eof()){
        readFile.getline(temp, 1023, 0);
        source += temp;
    }
    Node n = Parser::parse(source);
    Dom::printDom(n, 0);
}