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
    int q = 3;
    int b = 3;
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;

    GEOPolyhedron polyhedron;
    polyhedron.p = p;
    polyhedron.q = q;

    polyhedron.CreateStartingPolyhedron();
    Path minimumPath;
    // polyhedron.ExportPolyhedron(minimumPath);
    GEOPolyhedron tessellatedPolyhedron = TypeITessellation(polyhedron, b);
    tessellatedPolyhedron.ExportPolyhedron(minimumPath);
    // GEOPolyhedron finalPolyhedron = TypeIITessellation(polyhedron, tessellatedPolyhedron, b);
    // Path minimumPath;
    // OntoTheUnitSphere(finalPolyhedron);
    // finalPolyhedron.ExportPolyhedron(minimumPath);


    return 0;
}