# include <iostream>
# include <vector>
# include <cctype>
# include <chrono>
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
    GEOPolyhedron tessellatedPolyhedron;

    // check the validity for the values b and c and perform the requested tessellation
    if((b == 0 && c >= 1) || (b >= 1 && c == 0))
    {
        tessI = true;
		cout << "Tessellation type I" << endl;
		int n = max(b,c);
		tessellatedPolyhedron = TypeITessellation(polyhedron, n);
	} else if(b == c && b != 0)
    {
        tessII = true;
		cout << "Tessellation type II'" << endl;
		TypeIITessellation(polyhedron, b);
	} else 
		cout << "Invalid values for b and c" << endl;

    Gedim::UCDUtilities utilities;

    Eigen::VectorXi VerticesMarkers(tessellatedPolyhedron.NumVertices);
    for(int i = 0; i < tessellatedPolyhedron.NumVertices; i++)
    {
        VerticesMarkers[i] = 0;
    }
    
    Eigen::VectorXi EdgesMarkers(tessellatedPolyhedron.NumEdges);
    for(int i = 0; i < tessellatedPolyhedron.NumEdges; i++)
    {
        EdgesMarkers[i] = tessellatedPolyhedron.NumEdges - i;
    }
 

    utilities.ExportPoints("../PolygonalData/Cell0Ds.inp",
                            tessellatedPolyhedron.CoordVertices,
                            {},
                            VerticesMarkers);

    utilities.ExportSegments("../PolygonalData/Cell1Ds.inp",
                                tessellatedPolyhedron.CoordVertices,
                                tessellatedPolyhedron.ExtremaEdges,
                                {},
                                {},
                                EdgesMarkers);
    
    vector<vector<unsigned int>> FacesVertices;
    FacesVertices.resize(tessellatedPolyhedron.NumFaces);

    for(int i = 0; i < tessellatedPolyhedron.NumFaces; i++)
    {
        FacesVertices[i].resize(3);
        
        // Salvo i vertici di ciascuna faccia con indice "i" all'interno del vettore con indice "i"
        FacesVertices[i][0] = tessellatedPolyhedron.ListVertFaces(0, i);
        FacesVertices[i][1] = tessellatedPolyhedron.ListVertFaces(1, i);
        FacesVertices[i][2] = tessellatedPolyhedron.ListVertFaces(2, i);
    }

    Eigen::VectorXi FacesMarkers(tessellatedPolyhedron.NumFaces);
    for(int i = 0; i < tessellatedPolyhedron.NumFaces; i++)
    {
        FacesMarkers[i] = i;
    }

    utilities.ExportPolygons("../PolygonalData/Cell2Ds.inp",
                                tessellatedPolyhedron.CoordVertices,
                                FacesVertices,
                                {},
                                {},
                                FacesMarkers);

                                 
    // Constructor.CreateCells();
    // Constructor.ExportPolyhedra();
    // Dualise(polyhedron, p, q);
    cout << "tessellatedPolyhedron.NumVertices: " << tessellatedPolyhedron.NumVertices << endl;

    Path minimumPath;
    
    if(findMinPath)
    {
        if(id_vertex_1 >= 0 && id_vertex_1 < tessellatedPolyhedron.NumVertices && id_vertex_2 >= 0 && id_vertex_2 < tessellatedPolyhedron.NumVertices)
        {
            vector<int> minPath;
            // vector<int> minPath1;
            // Eigen::MatrixXd matrWeights = Eigen::MatrixXd::Zero(tessellatedPolyhedron.NumVertices, tessellatedPolyhedron.NumVertices);
            // for(int i = 0; i < tessellatedPolyhedron.NumVertices; i++){
            //     for(int j = 0; j < tessellatedPolyhedron.NumVertices; j++){
            //         if(tessellatedPolyhedron.MatrEdgeVertices(i,j) > -1){
            //             matrWeights(i,j) = tessellatedPolyhedron.lengthEdge;
            //         }
            //     }
            // }
            if(tessI)
            {
                minPath = BFS(tessellatedPolyhedron.AdjacencyList(), id_vertex_1, id_vertex_2, tessellatedPolyhedron.NumVertices, tessellatedPolyhedron.lengthEdge);

                // cout << "BFS" << endl;
                // int num_experiment = 500;
                // double time_elapsed_heap = 0.0;
                // for(unsigned int t = 0; t < num_experiment; t++)
                // {
                //     std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
                //     minPath = BFS(tessellatedPolyhedron.AdjacencyList(), id_vertex_1, id_vertex_2, tessellatedPolyhedron.NumVertices, tessellatedPolyhedron.lengthEdge);
                //     std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                //     time_elapsed_heap += std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
                    
                // }
                // time_elapsed_heap /= num_experiment;
                // cout << "Tempo medio: " << time_elapsed_heap << " µs" << endl;

                // time_elapsed_heap = 0.0;
                // cout << "Dijkstra" << endl;
                // for(unsigned int t = 0; t < num_experiment; t++)
                // {
                //     std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
                //     minPath1 = Dijkstra(tessellatedPolyhedron.AdjacencyList(), id_vertex_1, id_vertex_2, tessellatedPolyhedron.NumVertices, matrWeights);
                //     std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                //     time_elapsed_heap += std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
                    
                // }
                // time_elapsed_heap /= num_experiment;
                // cout << "Tempo medio: " << time_elapsed_heap << " µs" << endl;
            } 
            // else if(tessII)
            // {
            //     minPath = Dijkstra(tessellatedPolyhedron.AdjacencyList(), id_vertex_1, id_vertex_2, tessellatedPolyhedron.NumVertices);
            // }
            MinimumPath(minPath, tessellatedPolyhedron.MatrEdgeVertices, tessellatedPolyhedron.NumVertices, tessellatedPolyhedron.NumEdges, minimumPath.VerticesShortPath, minimumPath.EdgesShortPath);
        }
        else
            cout << "Invalid values for id_vertex_1 and id_vertex_2" << endl;
    }

    return 0;
}