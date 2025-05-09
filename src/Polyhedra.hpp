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
        int NumFaces = polyhedron.NumFaces;
        int NumEdges = polyhedron.NumEdges;
        int NumVertices = polyhedron.NumVertices;
    public:
        BuildPolyhedra(const int& p, const int& q)
        {
            polyhedron.NumFaces = 2*q / ((p*q) - (2*p) - (2*q));
            polyhedron.NumEdges = 2*p / ((p*q) - (2*p) - (2*q));
            polyhedron.NumVertices = NumEdges - NumFaces + 2;
            switch (p) // finds the correct polyhedron requested
            {
            case 3:
                switch (q)
                {
                case 3:
                    cout << "Your Polyhedron is a Tetrahedron with\n " <<
                    NumVertices << " Vertices\n" <<
                    NumEdges << " Edges\n" <<
                    NumFaces << " Faces\n" << endl;
                    break;

                case 4:
                    cout << "Your Polyhedron is a Octahedron with \n " <<
                    NumVertices << " Vertices\n" <<
                    NumEdges << " Edges\n" <<
                    NumFaces << " Faces\n" << endl;
                    break;
                
                case 5:
                    cout << "Your Polyhedron is a Icosahedron with \n " <<
                    NumVertices << " Vertices\n" <<
                    NumEdges << " Edges\n" <<
                    NumFaces << " Faces\n" << endl;
                    break;

                default:
                    cout << "Your tuple doesn't match any possyble polyhedra" << endl;
                    break;
                }
                break;
            
            case 4:
                if (q == 3)
                {
                    cout << "Your Polyhedron is a Cube with \n " <<
                    NumVertices << " Vertices\n" <<
                    NumEdges << " Edges\n" <<
                    NumFaces << " Faces\n" << endl;
                } else {
                    cout << "Your tuple doesn't match any possyble polyhedra" << endl;
                }
                break;
        
            case 5:
                if (q == 3)
                {
                    cout << "Your Polyhedron is a Icosahedron with \n " <<
                    NumVertices << " Vertices\n" <<
                    NumEdges << " Edges\n" <<
                    NumFaces << " Faces\n" << endl;
                } else {
                    cout << "Your tuple doesn't match any possyble polyhedra" << endl; 
                }

                break;
                
            default:
                cout << "Your tuple doesn't match any possyble polyhedra" << endl;
                break;
            }
        }

    };

}