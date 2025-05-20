/* File containing the body of the functions we'll use */

# include "Polyhedra.hpp"
# include "Utils.hpp"
# include <vector>

using namespace PolyhedraLibrary;
using namespace std;

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

Eigen::Vector3d FindBarycenter(Eigen::Vector3i& VertFace)
{
    GEOPolyhedron polyhedron;
    Eigen::Vector3d barycenter_coordinates = Eigen::Vector3d::Zero(3);
    double Coord_x = 0;
    double Coord_y = 0;
    double Coord_z = 0;

    // Finding the coordinates of each vertex 
    for (int i = 0; i < VertFace.size(); i++)
    {
        Coord_x += polyhedron.CoordVertices(VertFace(i), 0);
        Coord_y += polyhedron.CoordVertices(VertFace(i), 1);
        Coord_z += polyhedron.CoordVertices(VertFace(i), 2);
    }

    barycenter_coordinates = {Coord_x/3.0, Coord_y/3.0, Coord_z/3.0};

    return barycenter_coordinates;
}

void OntoTheUnitSphere(Eigen::Vector3d vertex)
{
    double norm = vertex.norm();
    if (norm > 1e-8) 
    {
        vertex /= norm;
    } else {
        vertex.setZero();
    }
}


vector<int> WhichIsTheMinimumPathBetween(int& id_vertex_1, int& id_vertex_2)
{
    vector<int> Path;
    return Path;
}