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
    struct Path 
    {
        std::vector<double> VerticesShortPath = {}; // Binary vector for the vertices used in the minimum path
        std::vector<double> EdgesShortPath = {}; // Binary vector for the edges used in the minimum path
    };

    struct GEOPolyhedron
    {
        int p, q;
        double lengthEdge = 0.0;

        int NumVertices = 0; // Number of vertices
        std::vector<int> IdVertices = {}; // Id of all vertices
        Eigen::MatrixXd CoordVertices = {}; // Coordinates of the vertices, 3 x NumVertices matrix

        int NumEdges = 0; // Number of Edges
        std::vector<int> IdEdges = {}; // Id of all vertices
        Eigen::MatrixXi ExtremaEdges = {}; // Extrema of each edge, 2 x NumEdges matrix
        Eigen::MatrixXi MatrEdgeVertices = {}; // Edge connecting each pair of extrema, NumVertices x NumVertices matrix
        
        int NumFaces = 0; // Number of faces 
        std::vector<int> IdFaces = {}; // Id of all faces
        Eigen::MatrixXi ListVertFaces = {}; // All the vertices of each face, p x NumFaces matrix
        Eigen::MatrixXi ListEdgeFaces = {}; // All the edges of each face, p x NumFaces matrix

        int IdPolyhedra = 0; // Id of all polyhedra
        vector<int> ListVertPolyhedra = {}; // All the vertices of each polyhedra
        vector<int> ListEdgePolyhedra = {}; // All the edges of each polyhedra
        vector<int> ListFacePolyhedra = {}; // All the faces of each polyhedra
        
        Eigen::MatrixXi ListAdjacentFaces = {}; // All the adjacent faces for the face with index i, 
                                                // where i is the index of the column of the matrix, p x NumFaces matrix
        vector<vector<unsigned int>> ListFacesWithVertex = {}; // All the faces containing the vertex that has the index of 
                                                               // the inner vector

        /* "FindAdjacentFaces" saves inside "ListAdjacentFaces" the adjacent faces to each face. 
        Each column contains the ids of the adjacent faces for the face that has the column index as id */
        void FindAdjacentFaces(); // Create one matrix as explained above

        /* Creating the matrix containing the vertices of each face as its column, which is "ListVertFaces"
        and the matrix containing the ids of the edges of each face, which is "ListEdgeFaces" 
        Inputs list:
        - verticesOnFace: vector containing the ids of the vertices for the face considered;
        - newFacesFound: integer representing how many faces of the polyhedron were already found;
        - vecVertFaces: structure containing the unique faces already found as an array made up by their 3 ids;
        - numAdjacentVertices: integer representing how many vertices are adjacent to each 
        vertex in the polyhedron */
        void FindFaces(vector<int>& verticesOnFace, 
                       int& newFacesFound, 
                       vector<array<int, 3>>& vecVertFaces, 
                       int& numAdjacentVertices);
        
        /* "AdjacencyList" finds for each vertex the adjacency list composed by the vertices of the same 
        face of the polyhedron
        Inputs list:  
        - verticesOnFace: vector containing the ids of the vertices for the face considered;
        - numAdjacentVertices: integer representing how many vertices are adjacent to each 
        vertex in the polyhedron */
        std::vector<vector<int>> AdjacencyList(vector<int>& verticesOnFace, 
                                               int& numAdjacentVertices);

        /* "FindFacesWithVertex" saves inside "ListFacesWithVertex" at each column the faces containing 
        the vertex whose index is that of the column */
        void FindFacesWithVertex();

        /* "ExportPolyhedron" exports the structures of the polyhedron using the code of Mr. Vicini 
        in order to create a file readable by ParaView */
        // void ExportPolyhedron();

        /* "ExportPolyhedronWithoutFaces" exports the structures of the polyhedron (except for the faces) using 
        the code of Mr. Vicini in order to create a file readable by ParaView */
        void ExportPolyhedronWithoutFaces();

        /* "ExportPolyhedronWithoutFaces" exports the structures of the polyhedron colouring them if they belong 
        to the minimum path whose extrema were given as input to the code using the code of Mr. Vicini 
        sin order to create a file readable by ParaView */
        void ExportPolyhedron(Path& minimumPath);
    };

}