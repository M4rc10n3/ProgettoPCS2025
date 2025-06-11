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
    int b = 5;
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;

    BuildPolyhedra Constructor(p, q); // create the structure of the Polyedron
    Constructor.DataPolyhedra();
    GEOPolyhedron polyhedron = Constructor.GetPolyhedron();
    // polyhedron.ExportPolyhedron();
    
    GEOPolyhedron tessellatedPolyhedron = TypeITessellation(polyhedron, b);
    // tessellatedPolyhedron.ExportPolyhedron();
    // tessellatedPolyhedron.FindFacesWithVertex();

    GEOPolyhedron dualPolyhedron = Dualise(tessellatedPolyhedron);
    OntoTheUnitSphere(dualPolyhedron);
    dualPolyhedron.ExportPolyhedronWithoutFaces();

    // int idVertex1 = 0;
    // int idVertex2 = 13;
    // BFS(polyhedron.AdjacencyList(), idVertex1, idVertex2, polyhedron.NumVertices, polyhedron.lengthEdge);

    return 0;
}