# include <iostream>
# include <vector>
# include <numeric>
# include "Sources.hpp"
# include "UCDUtilities.hpp"

using namespace PolyhedraLibrary;
using namespace std;
using namespace Eigen;

int main()
{
    int p = 3;
    int q = 4;
    int b = 6;
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;

    GEOPolyhedron polyhedron;
    polyhedron.p = p;
    polyhedron.q = q;

    polyhedron.CreateStartingPolyhedron();
    Path minimumPath;
    polyhedron.ExportPolyhedron(minimumPath);
    // GEOPolyhedron tessellatedPolyhedron = TypeITessellation(polyhedron, b);
    // GEOPolyhedron finalPolyhedron = TypeIITessellation(polyhedron, tessellatedPolyhedron, b);
    // Path minimumPath;
    // OntoTheUnitSphere(finalPolyhedron);
    // finalPolyhedron.ExportPolyhedron(minimumPath);


    return 0;
}