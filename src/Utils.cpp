/* File containing the body of the functions we'll use */

# include "Polyhedra.hpp"
# include <vector>

using namespace PolyhedraLibrary;
using namespace std;

GEOPolyhedron CreatePolyhedron(const int& p, const int& q)
{
    GEOPolyhedron polyhedron;
    return polyhedron;
}

void TypeITassellation(GEOPolyhedron& polyhedron)
{

}

void TypeIITassellation(GEOPolyhedron& polyhedron)
{

}

GEOPolyhedron Dualise(GEOPolyhedron& polyhedron)
{
    return polyhedron;
}

Eigen::Vector3d FindBarycenter(Eigen::Vector3i& ColumnOfListVertFaces)
{
    int& vertex_1 = ColumnOfListVertFaces(0);
    int& vertex_2 = ColumnOfListVertFaces(1);
    int& vertex_3 = ColumnOfListVertFaces(2);

    // Finding the coordinates of each vertex 
    



    Eigen::Vector3d barycenter_coordinates = Eigen::Vector3d::Zero(3);

    return barycenter_coordinates;
}

void OntoTheUnitSphere(vector<double>& vertex)
{

}

vector<int> WhichIsTheMinimumPathBetween(int& id_vertex_1, int& id_vertex_2)
{
    vector<int> Path;
    return Path;
}