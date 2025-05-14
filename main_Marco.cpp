# include <iostream>
# include "Polyhedra.hpp"
# include "Eigen/Eigen"

using namespace PolyhedraLibrary;
using namespace std;
using namespace Eigen;

int main(){

    int n = 10;
    Matrix3Xd m = MatrixXd::Zero(3, n);
    cout << "m: " << m << endl;
    cout << "Hello world" << endl;
    return 0;
}