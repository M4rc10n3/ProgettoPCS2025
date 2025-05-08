# include <iostream>
# include <vector>
# include <cctype>
# include "Polyhedra.hpp"

using namespace PolyhedraLibrary;
using namespace std;

int main(int argc, char* argv[])
{
    int p, q, b, c, id_vertex_1, id_vertex_2;
    string executable;
    vector<string> parameters;

    if (argc != 5 && argc != 7)
    {
        cout << "Invalid tuple" << endl;
        return 1;        
    }

    executable = argv[0];
    for (int i = 1; i < argc; i++)
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

    if (argc == 7)
    {
        id_vertex_1 = stoi(parameters[4]);
        id_vertex_2 = stoi(parameters[5]);
        cout << "Your tuple is :(p,q,b,c,id_vertex_1,id_vertex_2) = (" <<
        p << "," << 
        q << "," << 
        b << "," << 
        c << "," <<
        id_vertex_1 << "," <<
        id_vertex_2 << ")" << endl;
    } else {
        cout << " Your tuple is :(p,q,b,c) = (" <<
        p << "," << 
        q << "," << 
        b << "," << 
        c << ")" << endl; 
    }

    switch (p)
    {
    case 3:
        break;
    
    case 4:
    default:
        cout << "Your tuple don't match any possyble polyhedra" << endl;
        break;
    }

    return 0;
}