# include <iostream>
# include <fstream>
# include <vector>
# include <math.h>
# include "Polyhedra.hpp"
# include "Eigen/Eigen"
# include "BuildPolyhedra.hpp"

using namespace std;

namespace PolyhedraLibrary{
    BuildPolyhedra::BuildPolyhedra(const int& Schlafli_p, const int& Schlafli_q)
    {
        p = Schlafli_p;
        q = Schlafli_q;

        polyhedron.NumFaces = (4 * q) / ((2*p) - (p*q) + 2*q); // determines the number of Face with p,q 
        polyhedron.NumEdges = (p * polyhedron.NumFaces) / 2;
        polyhedron.NumVertices = (p * polyhedron.NumFaces) / q;

        NumFaces = polyhedron.NumFaces; 
        NumEdges = polyhedron.NumEdges;
        NumVertices = polyhedron.NumVertices;

        polyhedron.CoordVertices = Eigen::MatrixXd(NumVertices, 3);
        polyhedron.ExtremesEdges = Eigen::MatrixXi(NumEdges, 2);
        // polyhedron.ListEdgeFaces = Eigen::MatrixXi(NumEdges, NumFaces);
        // polyhedron.ListVertFaces = Eigen::MatrixXi(NumVertices, NumFaces);
        
    }

    void BuildPolyhedra::DataPolyhedra()
    {
        int edgeindex = 0;
        if ((p - 2) * (q - 2) < 4)
        {
            switch (p) // finds the correct polyhedron requested
            {
            case 3:
                switch (q)
                {
                case 3:
                    cout << "Your Polyhedron is a Tetrahedron with:\n";
                    Length_edge = 2 * sqrt(6) / 3;
                    polyhedron.CoordVertices << 0,0,1,
                                                2* sqrt(2)/3.0,0,-1.0/3,
                                                -7.0/(12*sqrt(2)),sqrt(23.0/32),-1.0/3,
                                                -7.0/(12*sqrt(2)),-sqrt(23.0/32),-1.0/3;
                    for (int j = 0; j < NumVertices - 1; j++)
                    {
                        for (int k = j + 1; k < NumVertices; k++)
                        {
                            polyhedron.ExtremesEdges(edgeindex,0) = j;
                            polyhedron.ExtremesEdges(edgeindex,1) = k;
                            edgeindex ++;
                        }
                    }
                    break;
                case 4:
                    cout << "Your Polyhedron is a Octahedron with: \n";
                    Length_edge = sqrt(2);
                    break;
                case 5:
                    cout << "Your Polyhedron is a Icosahedron with: \n";
                    Length_edge = 4 / sqrt(10 + 2*sqrt(5));
                    break;
                }
                break;
            case 4:
                if (q == 3)
                {
                    cout << "Your Polyhedron is a Cube with: \n";
                    Length_edge = 2 / sqrt(3);
                }
                break;
            case 5:
                if (q == 3)
                {
                    cout << "Your Polyhedron is a Dodecahedron with: \n";
                    Length_edge = 4 / (sqrt(3) * (1 + sqrt(5)));
                }
                break;
            }
            cout << NumVertices << " Vertices\n" <<
            NumEdges << " Edges\n" <<
            NumFaces << " Faces\n" << endl;
        }
    }

    void BuildPolyhedra::Cell0Ds()
    {   
        Eigen::MatrixXd CoordVertices = polyhedron.CoordVertices;
        // Eigen::MatrixXd CoordVertices = Eigen::MatrixXd::Zero(NumVertices, 3);
        
        ofstream file("../PolygonalData/Cell0Ds.txt"); // the program should be launched inside Debug or Release folders
        
        file << "Id,X,Y,Z\n";
        for (int i = 0; i < NumVertices; i++)
        {
            file << i << "," << CoordVertices(i, 0) << "," << 
            CoordVertices(i, 1) << "," << 
            CoordVertices(i, 2) << "\n";
            
            polyhedron.IdVertices.push_back("V" + to_string(i));
        }

        file.close();
    }

    void BuildPolyhedra::Cell1Ds()
    {   
        Eigen::MatrixXi ExtremesEdges = polyhedron.ExtremesEdges;
        // Eigen::MatrixXd ExtremesEdges = Eigen::MatrixXd::Zero(NumEdges, 2);
        ofstream file("../PolygonalData/Cell1Ds.txt"); // the program should be launched inside Debug or Release folders
        
        file << "Id,Origin,End\n";
        for (int i = 0; i < NumEdges; i++)
        {
            file << i << "," << ExtremesEdges(i,0) << "," << 
            ExtremesEdges(i,1) << "\n";
            
            polyhedron.IdEdges.push_back("E" + to_string(i));
        }

        file.close();
    }

    void BuildPolyhedra::Cell2Ds()
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

    void BuildPolyhedra::Cell3Ds()
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

    void BuildPolyhedra::CreateCells() // Function that calls all the other functions 
    {
        Cell0Ds();

        Cell1Ds();

        Cell2Ds();

        Cell3Ds();
    }


};
