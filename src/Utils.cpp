/* File containing the body of the functions we'll use */

# pragma once
# include "Polyhedra.hpp"
# include <vector>

using namespace PolyhedraLibrary;
using namespace std;

GEOPolyhedron CreatePolyhedron(const int& p, const int& q)
{
    GEOPolyhedron polyhedron;
    polyhedron.NumFaces = 4 * q / ((p - 2) * (q - 2) - 4);
    polyhedron.NumEdges = p * polyhedron.NumFaces / 2;
    polyhedron.NumVertices = p * polyhedron.NumFaces / q;
}

void TypeITassellation(GEOPolyhedron& polyhedron)
{

}

void TypeIITassellation(GEOPolyhedron& polyhedron)
{

}

GEOPolyhedron Dualise(GEOPolyhedron& polyhedron)
{

}

void OntoTheSphere(vector<double>& vertex)
{

}

Path WhichIsTheMinimumPathBetween(int& id_vertex_1, int& id_vertex_2)
{

}