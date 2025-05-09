/* File where we'll create the class or struct Polyhedra with all of its attributes */

# pragma once
# include <vector>
# include "Eigen/Eigen"

namespace PolyhedraLibrary{

    struct GEOPolyhedron
    {
        int NumVertices; // Number of vertices
        std::vector<int> IdVertices; // Id of all vertices
        Eigen::MatrixXd CoordVertices; // Coordinates of the vertices, 2 x NumVertices matrix

        int NumEdges; // Number of Edges
        std::vector<int> IdEdges; // Id of all vertices
        Eigen::MatrixXi ExtremesEdges; // Extremes of each edge, 2 x NumEdges matrix

        int NumFaces; // Number of faces 
        int VertFaces; // Number of vertices per face
        std::vector<int> IdFaces; // Id of all faces
        Eigen::MatrixXi ListVertFaces; // All the vertices of each face, VertFaces x NumFaces matrix
    };
    /* TODO Path can be useful if we want a particular structure for the minimum path between
    two vertices */
    struct Path {

    };

}