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
        std::vector<int> IdEdges = {}; // Id of all edges
        Eigen::MatrixXi ExtremaEdges = {}; // Extrema of each edge, 2 x NumEdges matrix
        Eigen::MatrixXi MatrEdgeVertices = {}; // Edge connecting each pair of extrema, NumVertices x NumVertices matrix
        
        int NumFaces = 0; // Number of faces 
        std::vector<int> IdFaces = {}; // Id of all faces
        Eigen::MatrixXi ListVertFaces = {}; // Vertices of each face, p x NumFaces matrix
        Eigen::MatrixXi ListEdgeFaces = {}; // Edges of each face, p x NumFaces matrix
        Eigen::MatrixXi ListAdjacentFaces = {}; // Adjacent faces for each face, p x NumFaces matrix
                                                
        /* "CreatePolyhedron" creates the starting polyhedron: this function can create a tetrahedron, octahedron or icosahedron */
        void CreateStartingPolyhedron();

        /* "DistanceSquaredBetween" is a method that calculates the squared distance between two vertices 
        or points just by using their ids.
        Inputs list:
        - idPoint1: id of the first point 
        - idPoint2: id of the second point */
        double DistanceSquaredBetween(int& idPoint1, 
                                      int& idPoint2);

        /* "AdjacencyList" finds for each vertex the adjacency list of vertices on the same face of the polyhedron.
        Inputs list:  
        - verticesOnFace: vector containing the ids of the vertices for the face considered;
        - numAdjacentVertices: integer representing how many vertices are adjacent to each 
                               vertex in the polyhedron */
        vector<vector<int>> AdjacencyList(vector<int>& verticesOnFace, 
                                          int& numAdjacentVertices);

        /* "FindFaces" creates the matrix containing the vertices of each face as its column, 
        which is "ListVertFaces" and the matrix containing the ids of the edges of each face, 
        which is "ListEdgeFaces" if we want to use it for a starting polyhedron. "FindFaces" can also 
        do the same things for the tessellated polyhedron, but in this case it does everything 
        on a single face of the starting polyhedron, so it needs the "verticesOnFace" as an argument.
        Inputs list:
        - verticesOnFace: vector containing the ids of the vertices for the face considered;
        - facesFound: integer representing how many faces of the polyhedron were already found;
        - vecVertFaces: structure containing the unique faces already found as an array made up by their 3 ids;
        - numAdjacentVertices: integer representing how many vertices are adjacent to each 
                               vertex in the polyhedron. */
        void FindFaces(vector<int>& verticesOnFace, 
                       int& facesFound, 
                       vector<array<int, 3>>& vecVertFaces, 
                       int& numAdjacentVertices);

        /* "FindEdges" creates the matrix containing the vertices of each edge as its column, 
        which is "ExtremaEdges" and the matrix containing the ids of the edges in position 
        (extremum1, extremum2), which is "MatrEdgeVertices" if we want to use it for a 
        starting polyhedron. "FindEdges" can also do the same things for the tessellated polyhedron, 
        but in this case it does everything on a single face of the starting polyhedron, 
        so it needs the "verticesOnFace" as an argument.
        Inputs list:
        - verticesOnFace: vector containing the ids of the vertices for the face considered;
        - edgesFound: integer representing how many edges of the polyhedron were already found. */
        void FindEdges(vector<int>& verticesOnFace, int& edgesFound);

        /* "FindAdjacentFaces" saves inside "ListAdjacentFaces" the adjacent faces to each face. 
        Each column contains the ids of the adjacent faces for the face that has the column index as id.
        We'll need it in order to avoid duplicating the edges and vertices in the tessellation of type I. */
        void FindAdjacentFaces();
        
        /* "ExportPolyhedron" exports the structures of the polyhedron colouring them and using 
        the code of Mr. Vicini in order to create a file readable by ParaView. 
        It also deals with the minimum path between the vertices that were given as input to the code. 
        Inputs list:
        - minimumPath: object of type Path containing the binary vectors that will be 
                       exported as the properties ShortPath;*/
        void ExportPolyhedron(Path& minimumPath);

        /* "CellXDs" export the data structures of the polyhedron on the files with extension .txt 
        required by the project */
        void Cell0Ds();
        void Cell1Ds();
        void Cell2Ds();
        void Cell3Ds();
    };

}