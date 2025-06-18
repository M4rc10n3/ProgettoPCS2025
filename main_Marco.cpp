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
    int q = 5;
    int b = 17;
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;

    BuildPolyhedra Constructor(p, q); // create the structure of the Polyedron
    Constructor.DataPolyhedra();
    Constructor.CreateCells();
    GEOPolyhedron polyhedron = Constructor.GetPolyhedron();
    // polyhedron.ExportPolyhedron();
    
    GEOPolyhedron tessellatedPolyhedron = TypeITessellation(polyhedron, b);
    // tessellatedPolyhedron.FindFacesWithVertex();
    Path minimumPath;
    OntoTheUnitSphere(tessellatedPolyhedron);
    tessellatedPolyhedron.ExportPolyhedron(minimumPath);

    // GEOPolyhedron dualPolyhedron = Dualise(tessellatedPolyhedron);

    return 0;
}