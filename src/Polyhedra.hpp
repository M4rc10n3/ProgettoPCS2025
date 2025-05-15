/* File where we'll create the class or struct Polyhedra with all of its attributes */

# pragma once
# include <iostream>
# include <fstream>
# include <vector>
# include "Eigen/Eigen"

using namespace std;

namespace PolyhedraLibrary{

    struct GEOPolyhedron
    {
        int NumVertices; // Number of vertices
        std::vector<string> IdVertices; // Id of all vertices
        Eigen::MatrixXd CoordVertices; // Coordinates of the vertices, 3 x NumVertices matrix

        int NumEdges; // Number of Edges
        std::vector<string> IdEdges; // Id of all vertices
        Eigen::Matrix2Xi ExtremesEdges; // Extremes of each edge, 2 x NumEdges matrix

        int NumFaces; // Number of faces 
        int VertFaces; // Number of vertices per face
        std::vector<string> IdFaces; // Id of all faces
        Eigen::MatrixXi ListVertFaces; // All the vertices of each face, NumVertices x NumFaces matrix
        Eigen::MatrixXi ListEdgeFaces; // All the edges of each face, NumEdges x NumFaces matrix
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

            polyhedron.NumFaces = (4 * q) / ((2*p) - (p*q) + 2*q); // determines the number of Face with p,q 
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
                    }
                    break;
                case 5:
                    if (q == 3)
                    {
                        cout << "Your Polyhedron is a Dodecahedron with \n";
                
                    }
                    break;
                }
                cout << NumVertices << " Vertices\n" <<
                NumEdges << " Edges\n" <<
                NumFaces << " Faces\n" << endl;
            }
        }
        void Cell0DS()
        {   
            // Eigen::MatrixXd CoordVertices = polyhedron.CoordVertices;
            Eigen::MatrixXd CoordVertices = Eigen::MatrixXd::Zero(3, NumVertices);
            
            ofstream file("../PolygonalData/Cell0Ds.txt"); // the program should be launched inside Debug or Release folders
            
            file << "Id,X,Y,Z\n";
            for (int i = 0; i < NumVertices; i++)
            {
                file << i << "," << CoordVertices(0,i) << "," << 
                CoordVertices(1,i) << "," << 
                CoordVertices(2,i) << "\n";
                
                polyhedron.IdVertices.push_back("V" + to_string(i));
            }

            file.close();
        }

        void Cell1Ds()
        {   
            // Eigen::MatrixXd ExtremesEdges = polyhedron.ExtremesEdges;
            Eigen::MatrixXd ExtremesEdges = Eigen::MatrixXd::Zero(2, NumEdges);
            ofstream file("../PolygonalData/Cell1Ds.txt"); // the program should be launched inside Debug or Release folders
            
            file << "Id,Origin,End\n";
            for (int i = 0; i < NumEdges; i++)
            {
                file << i << "," << ExtremesEdges(0,i) << "," << 
                ExtremesEdges(1,i) << "\n";
                
                polyhedron.IdEdges.push_back("E" + to_string(i));
            }

            file.close();
        }

        void Cell2Ds()
        {   
            // Eigen::Matrix3Xd ListVertFaces = polyhedron.ListVertFaces;
            // Eigen::Matrix3Xd ListEdgeFaces = polyhedron.ListEdgeFaces;
            Eigen::MatrixXd ListVertFaces = Eigen::MatrixXd::Zero(NumVertices, NumFaces);
            Eigen::MatrixXd ListEdgeFaces = Eigen::MatrixXd::Zero(NumEdges, NumFaces);
            ofstream file("../PolygonalData/Cell2Ds.txt"); // the program needs to be launched inside Debug or Release folders
            
            file << "Id,Vertices,Edges\n";
            for (int i = 0; i < NumFaces; i++)
            {
                file << i;
                for (int j = 0; j < NumVertices; j++)
                {
                    file << "," << ListVertFaces(j, i);
                }

                for (int k = 0; k < NumEdges; k++)
                {
                    file << "," << ListEdgeFaces(k, i);
                }
                file << "\n";

                polyhedron.IdFaces.push_back("F" + to_string(i));
            }

            file.close();
        }

        void Cell3DS()
        {   
            ofstream file("../PolygonalData/Cell3Ds.txt"); // the program should be launched inside Debug or Release folders
            
            file << "Id\n";
            for (int i = 0; i < NumVertices; i++)
            {
                file << polyhedron.IdVertices[i] << "\n";
            }

            for (int j = 0; j < NumEdges; j++)
            {
                file << polyhedron.IdEdges[j] << "\n";
            }

            for (int k = 0; k < NumFaces; k++)
            {
                file << polyhedron.IdFaces[k] << "\n";
            }

            file.close();
        }

        void CreateCells() // Function that calls all the other functions 
        {
            Cell0DS();

            Cell1Ds();

            Cell2Ds();

            Cell3DS();
        }


    };

}