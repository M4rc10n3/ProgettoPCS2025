# pragma once

# include "Polyhedra.hpp"

namespace PolyhedraLibrary{
    class BuildPolyhedra
    {
    private: 
    GEOPolyhedron polyhedron;
    unsigned int p, q;
    int& NumFaces = polyhedron.NumFaces; 
    int& NumEdges = polyhedron.NumEdges;
    int& NumVertices = polyhedron.NumVertices;
    double Length_edge;

    void PointsPolyhedra();  // Provides all the point of the polyhedron

    void FillStructPolyhedra(); // Fills all the structures of GEOPolyhedron
    
    void Cell0Ds(); // Create Cell0Ds 

    void Cell1Ds(); // Create Cell1Ds 

    void Cell2Ds(); // Create Cell2Ds

    void Cell3Ds(); // Create Cell3Ds

    public:
        BuildPolyhedra(const int& Schlafli_p, const int& Schlafli_q); // Initialize the class

        BuildPolyhedra(GEOPolyhedron& polyhedron); // Initialize the class 

        void DataPolyhedra(); // Gives all the important Data of the polyhedron

        void CreateCells(); // First fill the structure then calls all the CellXDs functions  

        void GetStructure(); // Export the strctures using the code of Mr. Vicini in order 
                             // to create a file readable by ParaView

    };

}