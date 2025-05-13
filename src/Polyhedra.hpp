/* File where we'll create the class or struct Polyhedra with all of its attributes */

# pragma once
# include <iostream>
# include <vector>
# include "Eigen/Eigen"

using namespace std;

namespace PolyhedraLibrary{

    struct GEOPolyhedron
    {
        int NumVertices; // Number of vertices
        std::vector<int> IdVertices; // Id of all vertices
        Eigen::MatrixXd CoordVertices; // Coordinates of the vertices, 2 x NumVertices matrix

        int NumEdges; // Number of Edges
        std::vector<int> IdEdges; // Id of all vertices
        Eigen::MatrixXi ExtremesEdges; // Extremes of each edge, 2 x NumEdges matrix

        int NumFaces; // Number of faces 
        int VertFaces; // Number of vertices per face
        std::vector<int> IdFaces; // Id of all faces
        Eigen::MatrixXi ListVertFaces; // All the vertices of each face, VertFaces x NumFaces matrix
    };
    /* TODO Path can be useful if we want a particular structure for the minimum path between
    two vertices */
    struct Path {

    };

    class BuildPolyhedra
    {
    private: 

    GEOPolyhedron polyhedron;
    int NumFaces, NumEdges, NumVertices;
    int p,q;

    public:
        BuildPolyhedra(const int& Schlafli_p, const int& Schlafli_q)
        {
            p = Schlafli_p;
            q = Schlafli_q;

            polyhedron.NumFaces = (4 * q) / ((2*p) - (p*q) + 2*q); // 4 * q / ((2*p) - (p*q) + 2*q)
            polyhedron.NumEdges = (p * polyhedron.NumFaces) / 2;
            polyhedron.NumVertices = (p * polyhedron.NumFaces) / q;

            NumFaces = polyhedron.NumFaces;
            NumEdges = polyhedron.NumEdges;
            NumVertices = polyhedron.NumVertices;
            
        }
        void DataPolyhedra()
        {
            if ((p - 2) * (q - 2) < 4)
            {
                switch (p) // finds the correct polyhedron requested
                {
                case 3:
                    switch (q)
                    {
                    case 3:
                        cout << "Your Polyhedron is a Tetrahedron with\n";
                        break;
                    case 4:
                        cout << "Your Polyhedron is a Octahedron with \n";
                        break;
                    case 5:
                        cout << "Your Polyhedron is a Icosahedron with \n";
                        break;
                    }
                case 4:
                    if (q == 3)
                    {
                        cout << "Your Polyhedron is a Cube with \n";
                        break;
                    }
                case 5:
                    if (q == 3)
                    {
                        cout << "Your Polyhedron is a Icosahedron with \n";
                        break;
                    }

                }
                cout << NumVertices << " Vertices\n" <<
                NumEdges << " Edges\n" <<
                NumFaces << " Faces\n" << endl;
            }
        }

    };

}