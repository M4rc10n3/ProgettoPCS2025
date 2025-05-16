# include <iostream>
# include <fstream>
# include <vector>
# include <set>
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
        polyhedron.MatrEdgeVertices = Eigen::MatrixXi::Zero(NumEdges, NumVertices);
        polyhedron.ListEdgeFaces = Eigen::MatrixXi(NumEdges, NumFaces);
        polyhedron.ListVertFaces = Eigen::MatrixXi(NumVertices, NumFaces);
        
    }

    void BuildPolyhedra::DataPolyhedra()
    {
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

    void BuildPolyhedra::PointsPolyhedra()
    {   
        DataPolyhedra();
        Eigen::MatrixXd& CoordVertices = polyhedron.CoordVertices;
        vector<double> x(NumVertices), y(NumVertices), z(NumVertices);

        x[0] = 0;
        y[0] = 0;
        z[0] = 1;

        x[1] = 0;
        y[1] = 0;
        z[1] = 1;
        
    }

    void BuildPolyhedra::FillStructPolyhedra()
    {   
        PointsPolyhedra();             
        Eigen::MatrixXi& MatrEdgeVertices = polyhedron.MatrEdgeVertices;

        unsigned int edgeIndex = 0;

        for (int i = 0; i < NumVertices - 1; i++)
        {
            for (int j = i + 1; j < NumVertices; j++)
            {
                polyhedron.ExtremesEdges(edgeIndex,0) = i;
                polyhedron.ExtremesEdges(edgeIndex,1) = j;
                edgeIndex++;
            }
        }

        edgeIndex = 0;

        for(int i = 0; i < NumVertices; i++)
        {
            for(int j = i; j < NumVertices; j++)
            {
                if(j == i || !(polyhedron.ExtremesEdges(edgeIndex,0) == i && polyhedron.ExtremesEdges(edgeIndex,1) == j))
                    MatrEdgeVertices(i,j) = -1;
                else
                { 
                    if(edgeIndex < NumEdges)
                    {
                        MatrEdgeVertices(i,j) = edgeIndex;
                        MatrEdgeVertices(j,i) = edgeIndex;
                        edgeIndex++;
                    }
                }
            }
        }
      
        // for(unsigned int i = 0; i < NumVertices; i++) // prints MatrEdgeVertices
        // {
        //     for(unsigned int j = 0; j < NumVertices; j++)
        //         cout << MatrEdgeVertices(i,j) << " ";
        //     cout << endl;
        // }

        unsigned int faceIndex = 0;

        vector<int> adjVert;

        set<int> vertFaces;
        vector<set<int>> vecVertFaces;

        int v1, v2, v3, j;

        for (int i = 0; i < NumVertices; i++)
        {
            adjVert.clear();
            j = 0;
            while (j < NumVertices && adjVert.size() <= q)
            {
                if(MatrEdgeVertices(i,j) >= 0 && adjVert.size() < q)
                {
                    adjVert.push_back(j);
                }
                j++;
            }
            for (unsigned int k = 1; k < adjVert.size(); k++)
            {
                v1 = adjVert[k-1];
                v2 = adjVert[k];

                if(MatrEdgeVertices(v1, v2) >= 0)
                {
                    vertFaces = {i, v1, v2};
                    
                    if(find(vecVertFaces.begin(), vecVertFaces.end(), vertFaces) == vecVertFaces.end())
                    {
                        polyhedron.ListVertFaces(0, faceIndex) = i;
                        polyhedron.ListVertFaces(1, faceIndex) = v1;
                        polyhedron.ListVertFaces(2, faceIndex) = v2;
                        faceIndex++;
                        vecVertFaces.push_back(vertFaces);
                    }
                }
            }
        }

        faceIndex = 0;

        for (set<int> vertFaces : vecVertFaces)
        {
            auto it = vertFaces.begin();
            int v1 = *it++;
            int v2 = *it++;
            int v3 = *it;

            polyhedron.ListEdgeFaces(0, faceIndex) = MatrEdgeVertices(v1,v2);
            polyhedron.ListEdgeFaces(1, faceIndex) = MatrEdgeVertices(v2,v3);
            polyhedron.ListEdgeFaces(2, faceIndex) = MatrEdgeVertices(v1,v3);
            faceIndex++;
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
        Eigen::MatrixXi ListVertFaces = polyhedron.ListVertFaces;
        Eigen::MatrixXi ListEdgeFaces = polyhedron.ListEdgeFaces;
        // Eigen::MatrixXi ListVertFaces = Eigen::MatrixXi::Zero(NumVertices, NumFaces);
        // Eigen::MatrixXi ListEdgeFaces = Eigen::MatrixXi::Zero(NumEdges, NumFaces);
        ofstream file("../PolygonalData/Cell2Ds.txt"); // the program needs to be launched inside Debug or Release folders
        
        file << "Id,NumVerices,Vertices,NumEdges,Edges\n";
        for (int i = 0; i < NumFaces; i++)
        {
            file << i << "," << p;
            for (int j = 0; j < p; j++)
            {
                file << "," << ListVertFaces(j, i);
            }
            
            file << "," << p;
            for (int k = 0; k < p; k++)
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

    void BuildPolyhedra::CreateCells() 
    {
        FillStructPolyhedra();

        Cell0Ds();

        Cell1Ds();

        Cell2Ds();

        Cell3Ds();
    }


};
