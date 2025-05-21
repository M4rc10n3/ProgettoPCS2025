/* File containing the body of the functions we'll use */

# include "Utils.hpp"
# include "Polyhedra.hpp"
# include "BuildPolyhedra.hpp"
# include <vector>

using namespace PolyhedraLibrary;
using namespace std;

Eigen::Vector3d FindBarycenter(Eigen::Vector3i& VertFace)
{
    GEOPolyhedron polyhedron;
    Eigen::Vector3d barycenter_coordinates = Eigen::Vector3d::Zero(3); // initialyze a vector for the final coordinates
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

Eigen::Vector3d OntoTheUnitSphere(Eigen::Vector3d vertex)
{
    double norm = vertex.norm();
    if (norm > 1e-16) // Avoids ZeroDivision errors
    {
        vertex /= norm;
    } else {
        vertex.setZero();
    }
    return vertex;
}

vector<int> WhichIsTheMinimumPathBetween(int& id_vertex_1, int& id_vertex_2)
{
    vector<int> Path;
    return Path;
}

GEOPolyhedron Dualise(GEOPolyhedron& polyhedron, const unsigned int& Schlafli_p, const unsigned int& Schlafli_q)
{
    GEOPolyhedron DualPolyhedron;

    const unsigned int& q = Schlafli_p;
    const unsigned int& p = Schlafli_q;

    DualPolyhedron.NumFaces = (4 * q) / ((2*p) - (p*q) + 2*q); // determines the number of faces using p and q 
    DualPolyhedron.NumEdges = (p * DualPolyhedron.NumFaces) / 2;
    DualPolyhedron.NumVertices = (p * DualPolyhedron.NumFaces) / q;

    // Reserves the exact amount of memory for the differents IDs
    polyhedron.IdVertices.reserve(DualPolyhedron.NumVertices);
    polyhedron.IdEdges.reserve(DualPolyhedron.NumEdges);
    polyhedron.IdFaces.reserve(DualPolyhedron.NumFaces);

    // Initialize all the Matrices 
    polyhedron.CoordVertices = Eigen::MatrixXd(3, DualPolyhedron.NumVertices);
    polyhedron.ExtremaEdges = Eigen::MatrixXi(2, DualPolyhedron.NumEdges);
    polyhedron.MatrEdgeVertices = Eigen::MatrixXi::Constant(DualPolyhedron.NumVertices, DualPolyhedron.NumVertices, -1);
    polyhedron.ListEdgeFaces = Eigen::MatrixXi(p, DualPolyhedron.NumFaces);
    polyhedron.ListVertFaces = Eigen::MatrixXi(p, DualPolyhedron.NumFaces);

    for (int i : polyhedron.IdFaces)
    {
        Eigen::Vector3i dual_vertex  = polyhedron.ListVertFaces.col(i);
        Eigen::Vector3d barycenter_coordinates = OntoTheUnitSphere(FindBarycenter(dual_vertex));
        DualPolyhedron.CoordVertices.row(i) = barycenter_coordinates;

    }

    return DualPolyhedron;
}

void TypeITassellation(GEOPolyhedron& polyhedron)
{

}

void TypeIITassellation(GEOPolyhedron& polyhedron)
{

}
