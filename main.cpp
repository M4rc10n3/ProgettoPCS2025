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

    bool findMinPath = false;

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
        findMinPath = true;
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

    bool tessI = false;
    bool tessII = false;

    // check the validity for the values b and c and perform the requested tessellation
    if((b == 0 && c >= 1) || (b >= 1 && c == 0))
    {
        tessI = true;
		cout << "Tessellation type I" << endl;
		int n = max(b,c);
		GEOPolyhedron tessellatedPolyhedron = TypeITessellation(polyhedron, n);
        
        // Nel caso in cui volessimo far visualizzare il poliedro tassellato da cui la dualizzazione 
        // parte, lasciare la prossima riga, altrikmenti è inutile
        tessellatedPolyhedron.ExportPolyhedron();

        GEOPolyhedron dualPolyhedron = Dualise(tessellatedPolyhedron);
        ontoTheUnitSphere(dualPolyhedron);
        dualPolyhedron.ExportPolyhedronWithoutFaces();
	} else if(b == c && b != 0)
    {
        tessII = true;
		cout << "Tessellation type II'" << endl;
		// TypeIITessellation(polyhedron, b);
	} 
    else 
    {
		cout << "Invalid values for b and c" << endl;
    }

    Path minimumPath;
    
    if(findMinPath)
    {
        if(id_vertex_1 >= 0 && id_vertex_1 < polyhedron.NumVertices && id_vertex_2 >= 0 && id_vertex_2 < polyhedron.NumVertices)
        {
            vector<int> minPath;
            if(tessI)
            {
                minPath = BFS(polyhedron.AdjacencyList(), id_vertex_1, id_vertex_2, polyhedron.NumVertices, polyhedron.lengthEdge);
            } 
            else if(tessII)
            {
                //minPath = Dijkstra(polyhedron.AdjacencyList(), id_vertex_1, id_vertex_2, polyhedron.NumVertices);
            }
            MinimumPath(minPath, polyhedron.MatrEdgeVertices, polyhedron.NumVertices, polyhedron.NumEdges, minimumPath.VerticesShortPath, minimumPath.EdgesShortPath);
        }
        else
            cout << "Invalid values for id_vertex_1 and id_vertex_2" << endl;
    }

    return 0;
}