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

    // Il nostro codice dovrebbe diventare così: 

    // GEOPolyhedron polyhedron;
    // if(p == 3)
    // {
    //     /* Following the directions of the project, we need to export the geodetic polyhedron 
    //     for the polyhedra with p = 3, so we start by creating the polyhedron that we'll tessellate 
    //     and then project on the unit sphere. The tetrahedron will follow this case: we'll 
    //     export its geodetic polyhedron, which is the same as its generalized Goldberg polyhedron */

    //     BuildPolyhedra Constructor(p, q); 
    //     Constructor.DataPolyhedra(); // create the structure of the Polyhedron
    //     polyhedron = Constructor.GetPolyhedron();
    // }
    // else if(q == 3)
    // {
    //     /* Following the directions of the project, we need to export the generalized Goldberg polyhedron 
    //     for the polyhedra with q = 3, so we decide to create the geodetic polyhedron 
    //     starting from the dual polyhedron of the polyhedron with q = 3 */
        
    //     BuildPolyhedra Constructor(q, p);
    //     Constructor.DataPolyhedra(); // create the structure of the Polyhedron
    //     polyhedron = Constructor.GetPolyhedron();
    // }

    BuildPolyhedra Constructor(p, q); 
    Constructor.DataPolyhedra(); // create the structure of the Polyhedron
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
        OntoTheUnitSphere(tessellatedPolyhedron);
	} 
    else if(b == c && b != 0)
    {
        tessII = true;
		cout << "Tessellation type II" << endl;
        GEOPolyhedron type_I_tesseletion = TypeITessellation(polyhedron, b);
		tessellatedPolyhedron = TypeIITessellation(polyhedron, type_I_tesseletion, b);
        // BuildPolyhedra TesConstructor(tessellatedPolyhedron);
        // TesConstructor.CreateCells();
        // OntoTheUnitSphere(tessellatedPolyhedron);
    } 
    else 
    {
		cout << "Invalid values for b and c" << endl;
    }

    cout << "tessellatedPolyhedron.NumVertices: " << tessellatedPolyhedron.NumVertices << endl;

    tessellatedPolyhedron.ExportPolyhedronWithoutFaces();
    
    
    vector<int> nullVec = {};
    int numAdjacentFaces = 6;
    Path minimumPath;

    if(findMinPath)
    {
        vector<int> minPath;
        double lengthPath;
        int num_experiment = 100;
        double time_elapsed_heap = 0.0;
        if(id_vertex_1 >= 0 && id_vertex_1 < tessellatedPolyhedron.NumVertices && id_vertex_2 >= 0 && id_vertex_2 < tessellatedPolyhedron.NumVertices)
        {
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
                cout << "BFS algorithm to find the minimum path" << endl;
                for(int t = 0; t < num_experiment; t++)
                {
                    lengthPath = 0.0;
                    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

                    minPath = BFS(tessellatedPolyhedron.AdjacencyList(nullVec, numAdjacentFaces), id_vertex_1, id_vertex_2, tessellatedPolyhedron.NumVertices, tessellatedPolyhedron.lengthEdge, lengthPath);

                    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                    time_elapsed_heap += std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();                    
                }
                time_elapsed_heap /= num_experiment;
                cout << "Average time to run the algorithm: " << time_elapsed_heap << " µs" << endl;            
            } 
            else if(tessII)
            {                
                cout << "Dijkstra algorithm to find the minimum path" << endl;
                // for(unsigned int t = 0; t < num_experiment; t++)
                // {
                //     lengthPath = 0.0;
                //     std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

                //     minPath = Dijkstra(tessellatedPolyhedron.AdjacencyList(), id_vertex_1, id_vertex_2, tessellatedPolyhedron.NumVertices, matrWeights, lengthPath);
                    
                //     std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
                //     time_elapsed_heap += std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();                    
                // }
                // time_elapsed_heap /= num_experiment;
                // cout << "Average time to run the algorithm: " << time_elapsed_heap << " µs" << endl;            
            }
            MinimumPath(minPath, tessellatedPolyhedron.MatrEdgeVertices, tessellatedPolyhedron.NumVertices, 
                        tessellatedPolyhedron.NumEdges, lengthPath, minimumPath.VerticesShortPath, 
                        minimumPath.EdgesShortPath);
    
        } else {
            cout << "Invalid values for id_vertex_1 and id_vertex_2" << endl;
        }
    }

    tessellatedPolyhedron.ExportPolyhedron(minimumPath);

    // Constructor.CreateCells();

    return 0;
}