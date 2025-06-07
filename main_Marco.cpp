# include <iostream>
# include <vector>
# include <numeric>
# include "Sources.hpp"
# include "Eigen/Eigen"
# include "UCDUtilities.hpp"

using namespace PolyhedraLibrary;
using namespace std;
using namespace Eigen;

int main(){

    int n = 10;
    Matrix3Xd m = MatrixXd::Zero(3, n);
    cout << "m: " << m << endl;
    Eigen::RowVector3d barycenter_coordinates;
    barycenter_coordinates << 1, 1, 1;
    cout << "barycenter_coordinates.normalized(): " << barycenter_coordinates.normalized() << endl;
    cout << "barycenter_coordinates: " << barycenter_coordinates << endl;
    vector<int> prova(5, 0);
    cout << "prova = { " << endl;
    for(auto elem : prova){
        cout << elem << " " << endl;
    }
    cout << "}" << endl;

    cout << "Hello world" << endl;

    int p = 3;
    int q = 3;
    int b = 3;
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;

    BuildPolyhedra Constructor(p, q); // create the structure of the Polyedron
    Constructor.DataPolyhedra();
    GEOPolyhedron polyhedron = Constructor.GetPolyhedron();
    
    GEOPolyhedron tessellatedPolyhedron = TypeITessellation(polyhedron, b);
    // tessellatedPolyhedron.FindFacesWithVertex();

    // int idVertex1 = 0;
    // int idVertex2 = 13;
    // BFS(polyhedron.AdjacencyList(), idVertex1, idVertex2, polyhedron.NumVertices, polyhedron.lengthEdge);

    Gedim::UCDUtilities utilities;
    Eigen::VectorXi VerticesMarkers(tessellatedPolyhedron.NumVertices);
    for(int i = 0; i < tessellatedPolyhedron.NumVertices; i++)
    {
        VerticesMarkers[i] = i;
    }
    Eigen::VectorXi EdgesMarkers(tessellatedPolyhedron.NumEdges);
    for(int i = 0; i < tessellatedPolyhedron. NumEdges; i++)
    {
        EdgesMarkers[i] = tessellatedPolyhedron. NumEdges - i;
    }
    Eigen::VectorXi FacesMarkers(tessellatedPolyhedron.NumFaces);
    for(int i = 0; i < tessellatedPolyhedron.NumFaces; i++)
    {
        FacesMarkers[i] = i;
    }

    // cout << "VerticesMarkers: \n" << VerticesMarkers << endl;
    // cout << "EdgesMarkers: \n" << EdgesMarkers << endl;
    // cout << "FacesMarkers: \n" << FacesMarkers << endl;

    utilities.ExportPoints("../PolygonalData/Cell0Ds.inp",
                            tessellatedPolyhedron.CoordVertices,
                            {},
                            VerticesMarkers);
    utilities.ExportSegments("../PolygonalData/Cell1Ds.inp",
                             tessellatedPolyhedron.CoordVertices,
                             tessellatedPolyhedron.ExtremaEdges,
                             {},
                             {},
                            EdgesMarkers);

    vector<vector<unsigned int>> FacesVertices;
    FacesVertices.resize(tessellatedPolyhedron.NumFaces);

    for(int i = 0; i < tessellatedPolyhedron.NumFaces; i++)
    {
        FacesVertices[i].resize(3);
        
        // Salvo i vertici di ciascuna faccia con indice "i" all'interno del vettore con indice "i"
        FacesVertices[i][0] = tessellatedPolyhedron.ListVertFaces(0, i);
        FacesVertices[i][1] = tessellatedPolyhedron.ListVertFaces(1, i);
        FacesVertices[i][2] = tessellatedPolyhedron.ListVertFaces(2, i);
    }

    utilities.ExportPolygons("../PolygonalData/Cell2Ds.inp",
                                tessellatedPolyhedron.CoordVertices,
                                FacesVertices,
                                {},
                                {},
                                FacesMarkers);
    // Constructor.ExportPolyhedra();
    // cout << VectorXd::LinSpaced(5,0.0,1.0).transpose() << endl;

    return 0;
}