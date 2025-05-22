/* File containing the body of the functions we'll use */

# include "Utils.hpp"
# include "Polyhedra.hpp"
# include "BuildPolyhedra.hpp"
# include <vector>

using namespace PolyhedraLibrary;
using namespace std;

Eigen::Vector3d FindBarycenter(GEOPolyhedron& polyhedron, Eigen::Vector3i& VertFace)
{
    Eigen::Vector3d barycenter_coordinates = Eigen::Vector3d::Zero(3); // initialyze a vector for the final coordinates
    double Coord_x = 0;
    double Coord_y = 0;
    double Coord_z = 0;

    // Finding the coordinates of each vertex 
    for (int i = 0; i < VertFace.size(); i++)
    {
        Coord_x += polyhedron.CoordVertices(0, VertFace(i));
        Coord_y += polyhedron.CoordVertices(1, VertFace(i));
        Coord_z += polyhedron.CoordVertices(2, VertFace(i));
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
    // Le due prossime righe servono a rimuovere solo i warning dalla compilazione
    id_vertex_1 = 0;
    id_vertex_2 = 0;

    vector<int> Path;
    return Path;
}

void Dualise(GEOPolyhedron& polyhedron, const unsigned int& Schlafli_p, const unsigned int& Schlafli_q)
{
    Eigen::MatrixXd CoordVertices(3, polyhedron.NumFaces);
    BuildPolyhedra constructor(Schlafli_q, Schlafli_p); // p, q are switched becasue it's the dual polyhedron

    cout << "\n DualPolyhedron \n" << endl;

    for (int i = 0; i < polyhedron.NumFaces; i++)
    {
        Eigen::Vector3i dual_vertex  = polyhedron.ListVertFaces.col(i);
        Eigen::Vector3d barycenter_coordinates = OntoTheUnitSphere(FindBarycenter(polyhedron, dual_vertex));
        CoordVertices.col(i) = barycenter_coordinates; 
    }

    constructor.PointsPolyhedra(CoordVertices);
    constructor.CreateCells();
}

// void TypeITassellation(GEOPolyhedron& polyhedron)
// {
    
// }

// void TypeIITassellation(GEOPolyhedron& polyhedron)
// {
    
// }
