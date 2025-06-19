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
    // TODO: da togliere prima di consegnare
    string executable;
    //
    vector<string> parameters;

    // TODO: da togliere prima di consegnare
    if (strcmp(argv[0], "gdb") == 0) // check if you're using gdb for Debug and avoids unnecessary input errors
    {
        argc--;
        argv++;
        executable = argv[0];
    }
    else {
        executable = argv[0];
    }
    //
    

    if(argc != 5 && argc != 7) // check if there is a compatible number of inputs 
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

    bool findMinPath = false; // boolean variable to know whether the program needs to find a minimum path

    if(argc == 7) // prints the tuple for the user to check
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

    /* Let's define a boolean we'll use to tell the program to stop if something's wrong */
    bool goOn = true;

    /* Let's initialise the starting polyhedron */
    GEOPolyhedron polyhedron;
    
    /* Let's check whether the inputs are correct: 
        - if that's the case, we can store p and q inside the respective polyhedron data structures 
          in order to create the starting polyhedron later;
        - if that's not the case, we print an error message and stop the program using 
          the boolean variable "goOn" */
    if(p == 3){
        polyhedron.p = p;
        if(q == 3){
            polyhedron.q = q;
            cout << "Your starting polyhedron is a Tetrahedron. " << endl;
        }
        else if(q == 4){
            polyhedron.q = q;
            cout << "Your starting polyhedron is a Octahedron. " << endl;
        }
        else if(q == 5){
            polyhedron.q = q;
            cout << "Your starting polyhedron is an Icosahedron. " << endl;
        }
        else{    
            goOn = false;
            cerr << "Invalid values for p and q" << endl;
            cerr << "This program cannot handle your polyhedron."  << endl;
            cerr << "This program only works with platonic polyhedra."  << endl;
            cerr << "If you want a platonic polyhedra with p = 3, try inputting 3, 4 or 5 for q." << endl << endl;
        }
    }
    else if(p == 4){
        if(q == 3){
            /* If the polyhedron requested is a cube, we start from a octahedron, which has the values 
            of p and q of the cube, but reversed */
            polyhedron.p = q;
            polyhedron.q = p;

            cout << "Your starting polyhedron is a Cube. " << endl;
        }
        else{
            goOn = false;
            cerr << "Invalid values for p and q" << endl;
            cerr << "This program cannot handle your polyhedron."  << endl;
            cerr << "This program only works with platonic polyhedra."  << endl;
            cerr << "If you want a platonic polyhedra with p = 4, try inputting 3 for q." << endl << endl;
        }
    }
    else if(p == 5){
        if(q == 3){
            /* If the polyhedron requested is a dodecahedron, we start from an icosahedron, which has 
            the values of p and q of the dodecahedron, but reversed */
            polyhedron.p = q;
            polyhedron.q = p;

            cout << "Your starting polyhedron is a Dodecahedron. " << endl;
        }
        else{
            goOn = false;
            cerr << "Invalid values for p and q" << endl;
            cerr << "This program cannot handle your polyhedron."  << endl;
            cerr << "This program only works with platonic polyhedra."  << endl;
            cerr << "If you want a platonic polyhedra with p = 5, try inputting 3 for q." << endl << endl;
        }
    }
    else{
        goOn = false;
        cerr << "Invalid values for p and q" << endl;
        cerr << "This program cannot handle your polyhedron."  << endl;
        cerr << "This program only works with platonic polyhedra."  << endl;
        cerr << "If you want a platonic polyhedra, you need to choose p and q among 3, 4 or 5." << endl << endl;
    }

    /* Let's "goOn" with the program only if everything's alright, which happens when "goOn" is true*/
    if(goOn){

        /* Let's create the starting polyhedron */
        polyhedron.CreateStartingPolyhedron();


        // tessellation
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
            GEOPolyhedron type_I_tessellation = TypeITessellation(polyhedron, b);
            tessellatedPolyhedron = TypeIITessellation(polyhedron, type_I_tessellation, b);
            OntoTheUnitSphere(tessellatedPolyhedron);
        } 
        else 
        {
            goOn = false;
            /* The program cannot find a minimum path if the polyhedron wasn't tessellated, 
            so let's set "findMinPath" to false*/
            findMinPath = false;
            /* Let's export the starting polyhedron if the tessellation never happened because of the inputs */
            polyhedron.ExportPolyhedron(minimumPath);
            cerr << "Invalid values for b and c" << endl;
            cerr << "This program can only handle positive integer values for b and c; \n" <<
                "in particular, just one of them can be equal to 0 or they can be the same value." << endl;
        }


        // minimum path finding
        Path minimumPath;
    
        if(findMinPath)
        {
            vector<int> minPath;
            double lengthPath;

            /* Let's define here two variables we'll need for the function 
            "AdjacencyList" used in the minimum path finding */
            vector<int> nullVec = {};
            int numAdjacentFaces = 6;
    
            if(id_vertex_1 >= 0 && id_vertex_1 < tessellatedPolyhedron.NumVertices 
                && id_vertex_2 >= 0 && id_vertex_2 < tessellatedPolyhedron.NumVertices)
            {
                if(tessI)
                {
                    cout << "BFS algorithm to find the minimum path" << endl;
    
                    minPath = BFS(tessellatedPolyhedron.AdjacencyList(nullVec, numAdjacentFaces), 
                                  id_vertex_1, 
                                  id_vertex_2, 
                                  tessellatedPolyhedron.NumVertices, 
                                  tessellatedPolyhedron.lengthEdge, 
                                  lengthPath);
              
                } 
                else if(tessII)
                {           
                    cout << "Dijkstra algorithm to find the minimum path" << endl;
    
                    Eigen::MatrixXd matrWeights = Eigen::MatrixXd::Zero(tessellatedPolyhedron.NumVertices, tessellatedPolyhedron.NumVertices);
                    for(int i = 0; i < tessellatedPolyhedron.NumVertices; i++){
                        for(int j = 0; j < tessellatedPolyhedron.NumVertices; j++){
                            if(tessellatedPolyhedron.MatrEdgeVertices(i,j) > -1){
                                matrWeights(i,j) = tessellatedPolyhedron.lengthEdge;
                            }
                        }
                    }
    
                    minPath = Dijkstra(tessellatedPolyhedron.AdjacencyList(nullVec, numAdjacentFaces), 
                                       id_vertex_1, 
                                       id_vertex_2, 
                                       tessellatedPolyhedron.NumVertices, 
                                       matrWeights, 
                                       lengthPath);      
                }
                MinimumPath(minPath, 
                            tessellatedPolyhedron.MatrEdgeVertices, 
                            tessellatedPolyhedron.NumVertices, 
                            tessellatedPolyhedron.NumEdges, 
                            lengthPath, 
                            minimumPath.VerticesShortPath, 
                            minimumPath.EdgesShortPath);
                
                /* Let's export the tessellated polyhedron with the proper minimum path */
                tessellatedPolyhedron.ExportPolyhedron(minimumPath);
            } else {

                cout << "Invalid values for id_vertex_1 and id_vertex_2" << endl;
                cout << "They should be between 0 and " << tessellatedPolyhedron.NumVertices << endl;
            }
        }
        else if(goOn){
            /* Let's export the tessellated polyhedron without the minimum path */
            tessellatedPolyhedron.ExportPolyhedron(minimumPath);
        }
    }

    return 0;
}