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
    int q = 5;
    int b = 5;
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;

    BuildPolyhedra Constructor(p, q); // create the structure of the Polyedron
    Constructor.DataPolyhedra();
    GEOPolyhedron polyhedron = Constructor.GetPolyhedron();
    TypeITessellation(polyhedron, b);
    Gedim::UCDUtilities utilities;
    utilities.ExportPoints("../PolygonalData/Cell0Ds.inp",
                               polyhedron.CoordVertices);
    utilities.ExportSegments("../PolygonalData/Cell1Ds.inp",
                                 polyhedron.CoordVertices,
                                 polyhedron.ExtremaEdges);
    vector<vector<unsigned int>> FacesVertices;
    FacesVertices.resize(polyhedron.NumFaces);

    for(int i = 0; i < polyhedron.NumFaces; i++)
    {
        FacesVertices[i].resize(3);
        
        // Salvo i vertici di ciascuna faccia con indice "i" all'interno del vettore con indice "i"
        FacesVertices[i][0] = polyhedron.ListVertFaces(0, i);
        FacesVertices[i][1] = polyhedron.ListVertFaces(1, i);
        FacesVertices[i][2] = polyhedron.ListVertFaces(2, i);
    }

    Eigen::VectorXi FacesMarkers(polyhedron.NumFaces);
    for(int i = 0; i < polyhedron.NumFaces; i++)
    {
        FacesMarkers[i] = i;
    }

    utilities.ExportPolygons("../PolygonalData/Cell2Ds.inp",
                                polyhedron.CoordVertices,
                                FacesVertices,
                                {},
                                {},
                                FacesMarkers);
    // Constructor.ExportPolyhedra();
    // cout << VectorXd::LinSpaced(5,0.0,1.0).transpose() << endl;

    return 0;
}