# pragma once

# include "Polyhedra.hpp"

namespace PolyhedraLibrary{
    class BuildPolyhedra
    {
    private: 
    
    void Cell0Ds(); // Create Cell0Ds 

    void Cell1Ds(); // Create Cell0Ds 

    void Cell2Ds(); // Create Cell0Ds

    void Cell3Ds(); // Create Cell0Ds

    GEOPolyhedron polyhedron;
    int NumFaces, NumEdges, NumVertices, p, q;
    double Length_edge;

    public:
        BuildPolyhedra(const int& Schlafli_p, const int& Schlafli_q); // initialyze the class

        void DataPolyhedra(); // Gives all the important Data of the Polyhedron

        void CreateCells(); // Calls all the CellXDs functions 

    };

}