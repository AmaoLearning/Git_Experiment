#include <iostream>
#include <fstream>
#include "field.h"
#include "units.h"
#include "engine.h"

using namespace std;

int main(int argc, char* argv[])
{

//    string filename = "map1.txt";
//    ifstream ifm;
//    ifm.open(filename.c_str());
//    if (!ifm) {
//        cout << "Cannot open the file: " << filename << endl;
//        return -1;
//    }
    Vector<Unit*> units;
    Field* f = loadMap(cin, units);
    //for (auto x: units) {cout << '\n' << x->getSide();}

    if (f == NULL) {
        cout << "Failed to load map!" << endl;
        return 0;
    }
    cout << *f;
    //play(*f, ifs, ofs);
//    play(*f, cin, cout,units);

    delete f;
    for (int i = 0; i < units.size(); i++) {delete units[i];}

    //ifm.close();
    return 0;
}
