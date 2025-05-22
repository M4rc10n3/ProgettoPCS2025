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
    Eigen::RowVector3d barycenter_coordinates = Eigen::RowVector3d::Zero(3);
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
    cout << "p = " << p << endl;
    cout << "q = " << q << endl;
    BuildPolyhedra polyhedron(p, q);
    polyhedron.CreateCells();
    polyhedron.ExportPolyhedra();
    return 0;
}