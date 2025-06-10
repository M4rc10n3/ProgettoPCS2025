# pragma once

# include "Polyhedra.hpp"

namespace PolyhedraLibrary{
    class BuildPolyhedra
    {
    private: 
    GEOPolyhedron polyhedron;
    int& p = polyhedron.p;
    int& q = polyhedron.q;
    int& NumFaces = polyhedron.NumFaces; 
    int& NumEdges = polyhedron.NumEdges;
    int& NumVertices = polyhedron.NumVertices;

    double Length_edge;
    
    Eigen::MatrixXd& CoordVertices = polyhedron.CoordVertices;
    Eigen::MatrixXi& ExtremaEdges = polyhedron.ExtremaEdges;
    Eigen::MatrixXi& MatrEdgeVertices = polyhedron.MatrEdgeVertices;
    Eigen::MatrixXi& ListVertFaces = polyhedron.ListVertFaces;
    Eigen::MatrixXi& ListEdgeFaces = polyhedron.ListEdgeFaces;
    Eigen::MatrixXi& ListAdjacentFaces = polyhedron.ListAdjacentFaces;
    
    /* Creating the matrix containing the vertices of each edge and the matrix with the ids 
        of each edge at the coordinates i and j, where i and j are its two vertices.
        We do it by checking the distance between a vertex and all of the others, by using a for cycle 
        that doesn't check for the last two vertices (that iteration would be useless) */
    void NumberEdges(); // Create two matrices: one with the vertex IDs of each edge and one with
                                        // the edge IDs of each pair of vertices

    /* Creating the matrix containing the vertices of each face as its column and the matrix containing the ids 
        of the edges of each face. */
    void NumberFaces(); // Create two matrices: one with the vertex IDs and one with the edge IDs, 
                                        // which uniquely identify each face of the polyhedron
    
    /* Creating the matrix containing the adjacent faces of each face. 
    Each column contains the ids of the adjacent faces for the face that has as id the column index */
    // void FindAdjacentFaces(); // Create one matrix as explained above

    

    void Cell0Ds(); // Create Cell0Ds 

    void Cell1Ds(); // Create Cell1Ds 

    void Cell2Ds(); // Create Cell2Ds

    void Cell3Ds(vector<GEOPolyhedron>& allPolyhedra); // Create Cell3Ds

    public:
        BuildPolyhedra(const int& Schlafli_p, const int& Schlafli_q); // Initialize the class

        void DataPolyhedra(); // Gives all the important Data of the polyhedron

        void PointsPolyhedra(Eigen::MatrixXd& CoordVertices);  // Fills all the point of the polyhedron

        // vector<vector<int>> AdjacencyList(); // Creates the adjacency list, where each vertex is associated
                                             // with a vector containing all vertices adjacent to it

        void FillStructPolyhedra(); // Fills all the structures of GEOPolyhedron

        void CreateCells(vector<GEOPolyhedron>& allPolyhedra); // First fill the structure then calls all the CellXDs functions  

        GEOPolyhedron GetPolyhedron(); // Returns the final polyhedron requested 
    };

}