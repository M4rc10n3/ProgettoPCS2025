# pragma once

# include "Polyhedra.hpp"

namespace PolyhedraLibrary{
    class BuildPolyhedra
    {
    private: 

    void FillStructPolyhedra(); // Fill all the Structures of GEOPolyhedron
    
    void Cell0Ds(); // Create Cell0Ds 

    void Cell1Ds(); // Create Cell1Ds 

    void Cell2Ds(); // Create Cell2Ds

    void Cell3Ds(); // Create Cell3Ds

    GEOPolyhedron polyhedron;
    int NumFaces, NumEdges, NumVertices, p, q;
    double Length_edge;

    public:
        BuildPolyhedra(const int& Schlafli_p, const int& Schlafli_q); // Initialyze the class

        void DataPolyhedra(); // Gives all the important Data of the Polyhedron

        void CreateCells(); // First fill the structure than calls all the CellXDs functions  

    };

}