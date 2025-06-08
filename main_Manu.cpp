# include <iostream>
# include <vector>
# include <cctype>
# include "UCDUtilities.hpp"
# include "Sources.hpp"
# include "Eigen/Core"

using namespace PolyhedraLibrary;
using namespace std;

int main(int argc, char* argv[])
{
    // unsigned int p, q, b, c, id_vertex_1, id_vertex_2; // initialize all the variables
    int p, q, b, c, id_vertex_1, id_vertex_2; // initialize all the variables
    string executable;
    vector<string> parameters;

    if (strcmp(argv[0], "gdb") == 0) // check if you're using gdb for Debug and avoids unnecessary input errors
    {
        argc--;
        argv++;
        executable = argv[0];
    }
    else {
        executable = argv[0];
    }
    

    if (argc != 5 && argc != 7) // check if there is a compatible number of inputs 
    {
        cout << "Invalid tuple" << endl;
        return 1;        
    }

    for (int i = 1; i < argc; i++) // check that every input is a number 
    {
        string arg = argv[i];
        for (char ch : arg)
            if (!isdigit(ch))
            {
                cout << "Invalid tuple" << endl;
                return 1;
            }
        parameters.push_back(arg);
    }

    p = stoi(parameters[0]);
    q = stoi(parameters[1]);
    b = stoi(parameters[2]);
    c = stoi(parameters[3]);

    if (argc == 7) // prints the tuple 
    {
        id_vertex_1 = stoi(parameters[4]);
        id_vertex_2 = stoi(parameters[5]);
        cout << "Your tuple is: (p,q,b,c,id_vertex_1,id_vertex_2) = (" <<
        p << "," << 
        q << "," << 
        b << "," << 
        c << "," <<
        id_vertex_1 << "," <<
        id_vertex_2 << ")" << endl;
    } else {
        cout << "Your tuple is: (p,q,b,c) = (" <<
        p << "," << 
        q << "," << 
        b << "," << 
        c << ")" << endl; 
    }

    BuildPolyhedra Constructor(p, q); // create the structure of the Polyhedron
    Constructor.DataPolyhedra();
    GEOPolyhedron polyhedron = Constructor.GetPolyhedron();
    Path MinimumPath;
    // TypeITessellation(polyhedron, b);

    // Gedim::UCDUtilities utilities;

    // Eigen::VectorXi VerticesMarkers(polyhedron.NumVertices);
    // for(int i = 0; i < polyhedron.NumVertices; i++)
    // {
    //     VerticesMarkers[i] = 0;
    // }
    // cout << "VerticesMarkers: \n" << VerticesMarkers << endl;
    
    // Eigen::VectorXi EdgesMarkers(polyhedron.NumEdges);
    // for(int i = 0; i < polyhedron.NumEdges; i++)
    // {
    //     EdgesMarkers[i] = 10;
    // }
    // cout << "EdgesMarkers: \n" << EdgesMarkers << endl;
 

    // utilities.ExportPoints("../PolygonalData/Cell0Ds.inp",
    //                         polyhedron.CoordVertices,
    //                         {},
    //                         VerticesMarkers);

    // utilities.ExportSegments("../PolygonalData/Cell1Ds.inp",
    //                             polyhedron.CoordVertices,
    //                             polyhedron.ExtremaEdges,
    //                             {},
    //                             {},
    //                             EdgesMarkers);
    
    // vector<vector<unsigned int>> FacesVertices;
    // FacesVertices.resize(polyhedron.NumFaces);

    // for(int i = 0; i < polyhedron.NumFaces; i++)
    // {
    //     FacesVertices[i].resize(3);
        
    //     // Salvo i vertici di ciascuna faccia con indice "i" all'interno del vettore con indice "i"
    //     FacesVertices[i][0] = polyhedron.ListVertFaces(0, i);
    //     FacesVertices[i][1] = polyhedron.ListVertFaces(1, i);
    //     FacesVertices[i][2] = polyhedron.ListVertFaces(2, i);
    // }

    // Eigen::VectorXi FacesMarkers(polyhedron.NumFaces);
    // for(int i = 0; i < polyhedron.NumFaces; i++)
    // {
    //     FacesMarkers[i] = i;
    // }

    // utilities.ExportPolygons("../PolygonalData/Cell2Ds.inp",
    //                             polyhedron.CoordVertices,
    //                             FacesVertices,
    //                             {},
    //                             {},
    //                             FacesMarkers);

                                 
    Constructor.CreateCells();
    // Constructor.ExportPolyhedra();
    // Dualise(polyhedron, p, q);

    return 0;
}