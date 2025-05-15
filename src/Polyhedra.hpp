/* File where we'll create the class or struct Polyhedra with all of its attributes */

# pragma once
# include <iostream>
# include <fstream>
# include <vector>
# include <math.h>
# include "Eigen/Eigen"

using namespace std;

namespace PolyhedraLibrary
{

    struct GEOPolyhedron
    {
        int NumVertices; // Number of vertices
        std::vector<string> IdVertices; // Id of all vertices
        Eigen::MatrixXd CoordVertices; // Coordinates of the vertices, NumVertices x 3 matrix

        int NumEdges; // Number of Edges
        std::vector<string> IdEdges; // Id of all vertices
        Eigen::MatrixXi ExtremesEdges; // Extremes of each edge, NumEdges x 2 matrix

        int NumFaces; // Number of faces 
        int VertFaces; // Number of vertices per face
        std::vector<string> IdFaces; // Id of all faces
        Eigen::MatrixXi ListVertFaces; // All the vertices of each face, NumVertices x NumFaces matrix
        Eigen::MatrixXi ListEdgeFaces; // All the edges of each face, NumEdges x NumFaces matrix
    };
    /* TODO Path can be useful if we want a particular structure for the minimum path between
    two vertices */
    struct Path {
    };

}