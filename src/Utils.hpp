/* File containing the name and a little description of the functions we'll use */

# pragma once
# include "Polyhedra.hpp"

using namespace PolyhedraLibrary;

/* CreatePolyhedron is a function that creates the Polyhedron starting from the input data p and q
p: number of vertices of each face of the polyhedron
q: valence of the polyhedron */
Polyhedron CreatePolyhedron(int& p, int& q);