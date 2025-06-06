/* File containing the name and a little description of the functions we'll use */

# pragma once
# include "Polyhedra.hpp"
# include <vector>

using namespace PolyhedraLibrary;
using namespace std;

/* FindBarycenter is a function that outputs a vector containing the coordinates of the barycenter 
of each triangular face
Inputs list:
ListVertFace: object of type "MatrixXi" containing the ids of each vertex of the face */
Eigen::Vector3d FindBarycenter(GEOPolyhedron& polyhedron, Eigen::Vector3i& VertFace);

/*BFS is a function that finds the minimum path between the two vertices marked by the ids given as inputs
using the BFS algorithm (for unweighted graphs, therefore useful for type I tessellation)
Inputs list:
adjList: adjacency list that contains the adjacent vertices for each vertex in the polyhedron
v1: the id associated to the first vertex of the minimum path we're looking for 
v2: the id associated to the last vertex of the minimum path we're looking for 
n: number of vertices of the polyhedron
lengthEdge: length of the edges of the polyhedron (all equal to each other) */
vector<int> BFS(const vector<vector<int>>& adjList, const int& v1, const int& v2, const int& n, const double& lengthEdge);

/* Dualise is a function that outputs the dual, expressed as the type "Polyhedron", of a <polyhedron> given as input 
Inputs list:
polyhedron: object of type "Polyhedron" that the function uses to create its dual */
void Dualise(GEOPolyhedron& polyhedron, const int& Schlafli_p, const int& Schlafli_q);
// Che sia meglio dare come output il tipo void? Se facciamo return Polyhedron sarà una grossa e lenta operazione, vero? 
// Forse è meglio definire un nuovo poliedro prima di chiamare questa funzione e mettiamo come altro input della funzione il poliedro stesso in cui dobbiamo fare output?

/* TypeITassellation is a function that modifies the attributes of the object <polyhedron> in order to 
output its tassellations of type I regarding geodetic polyhedra 
Inputs list:
polyhedron: object of type "Polyhedron" that the function modifies. */
void TypeITessellation(GEOPolyhedron& polyhedron, int& numberDivisions);

/* TypeIITassellation is a function that modifies the attributes of the object <polyhedron> in order to 
output its tassellations of type II regarding geodetic polyhedra 
Inputs list:
polyhedron: object of type "Polyhedron" that the function modifies. */
void TypeIITessellation(GEOPolyhedron& polyhedron);

/* distanceBetween id a method that calculates the squared distance between two vertices or points 
just by using their ids 
Inputs list:
polyhedron: the polyhedron to which the points belong
idPoint1: id of the first point 
idPoint2: id of the second point */
double distanceSquaredBetween(GEOPolyhedron& polyhedron, int& idPoint1, int& idPoint2);

/* triangularNumber is a function that returns the b-th triangular number usign the Gauss formula 
Inputs list:
b: order of the triangular number, used as its seed */
int triangularNumber(int& b);
