#include <iostream>
#include <fstream>
#include "dom.h"
#include "parser.h"
#include "style.h"
#include "util.h"
#include "layout.h"
#include "render.h"
#include "CImg.h"

using namespace std;
using namespace berners;
using namespace cimg_library_suffixed;

string read(const string& path){
    ifstream readFile1(path);
    char temp[1024];
    string source = "";
    while(!readFile1.eof()){
        readFile1.getline(temp, 1023, 0);
        source += temp;
    }
    return source;
}

int main(){
    float width, height;
    std::cin >> width >> height;
    
    Node n = HtmlParser::parse(read("test.html"));
    Stylesheet s = CssParser::parse(read("test.css"));

    Style style;
    StyledNode node = style.style_tree(n, s);

    berners::Dimensions initial_containing_block(berners::Rect(0.0,0.0,width,height));
    LayoutBox root = Layout::layout_tree(node, initial_containing_block);
    Canvas canvas = Render::paint(root, initial_containing_block.content);

    CImg<unsigned char> visu((int)width,(int)height,1,3,0);

    CImgDisplay draw_disp(visu,"Intensity profile");
    for(uint32_t i = 0; i < (uint32_t)height; i++){
        int base = i * ((int)width);
        for(uint32_t j = 0; j < (uint32_t)width; j++){
            int index = base + j;
            const unsigned char color[] = { canvas.pixels[index].r,canvas.pixels[index].g,canvas.pixels[index].b};
            visu.draw_point(j,i,0,color);
        }
    }
    visu.display(draw_disp);

    system("pause");
    return 0;
}