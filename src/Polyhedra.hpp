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
        int p, q;
        double lengthEdge;

        int NumVertices; // Number of vertices
        std::vector<int> IdVertices; // Id of all vertices
        Eigen::MatrixXd CoordVertices; // Coordinates of the vertices, 3 x NumVertices matrix

        int NumEdges; // Number of Edges
        std::vector<int> IdEdges; // Id of all vertices
        Eigen::MatrixXi ExtremaEdges; // Extrema of each edge, NumEdges x 2 matrix
        Eigen::MatrixXi MatrEdgeVertices; // Edge connecting each pair of extrema, NumVertices x NumVertices matrix
        
        int NumFaces; // Number of faces 
        std::vector<int> IdFaces; // Id of all faces
        Eigen::MatrixXi ListVertFaces; // All the vertices of each face, p x NumFaces matrix
        Eigen::MatrixXi ListEdgeFaces; // All the edges of each face, p x NumFaces matrix
        
        Eigen::MatrixXi ListAdjacentFaces; // All the adjacent faces for the face with index i, 
                                           // where i is the index of the column of the matrix, p x NumFaces matrix
        vector<vector<unsigned int>> ListFacesWithVertex; // All the faces containing the vertex that has the index of 
                                                 // the inner vector

        /* "FindAdjacentFaces" saves inside "ListAdjacentFaces" the adjacent faces to each face. 
        Each column contains the ids of the adjacent faces for the face that has the column index as id */
        void FindAdjacentFaces(); // Create one matrix as explained above
        
        /* Finds the adjacency list of the vertices of the polyhedron */
        vector<vector<int>> AdjacencyList();

        /* "FindFacesWithVertex" saves inside "ListFacesWithVertex" at each column the faces containing 
        the vertex whose index is that of the column */
        void FindFacesWithVertex();

        /* "ExportPolyhedra" exports the structures of the polyhedron using the code of Mr. Vicini 
        in order to create a file readable by ParaView */
        void ExportPolyhedron();

        /* "ExportPolyhedra" exports the structures of the polyhedron (except for the faces) using 
        the code of Mr. Vicini in order to create a file readable by ParaView */
        void ExportPolyhedronWithoutFaces();
    };

    struct Path 
    {
        std::vector<int> VerticesShortPath; // Binary vector for the vertices used in the minimum path
        std::vector<int> EdgesShortPath; // Binary vector for the edges used in the minimum path
    };

}