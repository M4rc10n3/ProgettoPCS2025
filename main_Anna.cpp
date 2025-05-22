# include <iostream>
# include "Sources.hpp"

using namespace PolyhedraLibrary;
using namespace std;

int main(int argc, char * argv[])
{
    cout << "Hello world" << endl;

    int p = stoi(argv[1]);
    int q = stoi(argv[2]);
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;
    BuildPolyhedra polyhedron(p, q);
    polyhedron.CreateCells();
    polyhedron.ExportPolyhedra();
    
    return 0;
}