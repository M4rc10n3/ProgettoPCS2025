# include <iostream>
# include "Sources.hpp"
# include "Eigen/Eigen"

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
    int q = 4;
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;

    BuildPolyhedra Constructor(p, q); // create the structure of the Polyedron
    Constructor.DataPolyhedra();
    GEOPolyhedron polyhedron = Constructor.GetPolyhedron();
    Constructor.ExportPolyhedra();
    return 0;
}