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
Eigen::Vector3d findBarycenter(GEOPolyhedron& polyhedron, Eigen::Vector3i& VertFace);

void ontoTheUnitSphere(GEOPolyhedron& polyhedron);

/* BFS is a function that finds the minimum path between the two vertices marked by the ids given as inputs
using the BFS algorithm (for unweighted graphs, therefore useful for type I tessellation)
Inputs list:
adjList: adjacency list that contains the adjacent vertices for each vertex in the polyhedron
v1: the id associated to the first vertex of the minimum path we're looking for 
v2: the id associated to the last vertex of the minimum path we're looking for 
numVert: number of vertices of the polyhedron
lengthEdge: length of the edges of the polyhedron (all equal to each other) */
vector<int> BFS(const vector<vector<int>>& adjList, const int& v1, const int& v2, const int& numVert, const double& lengthEdge);

/* Dijkstra is a function that finds the minimum path between the two vertices marked by the ids given as inputs
using the Dijkstra algorithm (for weighted graphs, therefore useful for type II tessellation)
Inputs list:
adjList: adjacency list that contains the adjacent vertices for each vertex in the polyhedron
v1: the id associated to the first vertex of the minimum path we're looking for 
v2: the id associated to the last vertex of the minimum path we're looking for 
numVert: number of vertices of the polyhedron
matrWeights: matrix with the lengths of each edge of the polyhedron */
// vector<int> Dijkstra(const vector<vector<int>>& adjList, const int& v1, const int& v2, const int& numVert, Eigen::MatrixXd& matrWeights);

/* MinimumPath is a function that sets the property ShortPath = 1 to the vertices and the edges that compose 
the minimum path and ShortPath = 0 to the other vertices and edges
Inputs list:
minPath: vector with the ids of the vertices that compose the minimum path (created with BFS or Dijkstra algorithm)
MatrEdgeVertices: matrix of the edges connecting each pair of extrema
numVert: number of vertices of the polyhedron
numEdge: number of edges of the polyhedron
vertShortPath: binary vector for the vertices used in the minimum path
edgeShortPath: binary vector for the edges used in the minimum path */
void MinimumPath(const vector<int>& minPath, const Eigen::MatrixXi& MatrEdgeVertices, const int& numVert, const int& numEdge, vector<int>& vertShortPath, vector<int>& edgeShortPath);

/* Dualise is a function that outputs the dual, expressed as the type "Polyhedron", of a <polyhedron> given as input 
Inputs list:
polyhedron: object of type "Polyhedron" that the function uses to create its dual */
GEOPolyhedron Dualise(GEOPolyhedron& polyhedron);
// Che sia meglio dare come output il tipo void? Se facciamo return Polyhedron sarà una grossa e lenta operazione, vero? 
// Forse è meglio definire un nuovo poliedro prima di chiamare questa funzione e mettiamo come altro input della funzione il poliedro stesso in cui dobbiamo fare output?

/* TypeITassellation is a function that modifies the attributes of the object <polyhedron> in order to 
output its tassellations of type I regarding geodetic polyhedra 
Inputs list:
polyhedron: object of type "Polyhedron" that the function modifies. */
GEOPolyhedron TypeITessellation(GEOPolyhedron& polyhedron, int& numberDivisions);

/* TypeIITassellation is a function that modifies the attributes of the object <polyhedron> in order to 
output its tassellations of type II regarding geodetic polyhedra 
Inputs list:
polyhedron: object of type "Polyhedron" that the function modifies.
numberDivisions */
// void TypeIITessellation(GEOPolyhedron& polyhedron, int& numberDivisions);

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
