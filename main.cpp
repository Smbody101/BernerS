#include "dom.h"

using namespace std;
using namespace berners;

int main(){
    Node a("world");
    vector<Node> av;
    av.push_back(a);
    AttrMap aa;
    Node em("em", aa, av);
    
    Node b("Hello ");
    Node c("!");
    vector<Node> bv;
    bv.push_back(b);
    bv.push_back(em);
    bv.push_back(c);
    Node p("p", aa, bv);

    vector<Node> cv;
    cv.push_back(p);
    AttrMap ba;
    ba["id"] = "main";
    ba["class"] = "test";

    Node div("div", ba, cv);
    Node d("Title");
    vector<Node> dv;
    dv.push_back(d);
    Node h1("h1", aa, dv);

    vector<Node> ev;
    ev.push_back(h1);
    ev.push_back(div);
    Node body("body", aa, ev);

    vector<Node> fv;
    fv.push_back(body);
    Node html("html", aa, fv);

    Dom::printDom(html, 0);

}