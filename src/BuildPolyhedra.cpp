# include <iostream>
# include <fstream>
# include <vector>
# include <set>
# include <math.h>
# include <algorithm>
# include "Eigen/Eigen"
# include "Polyhedra.hpp"
# include "BuildPolyhedra.hpp"
# include "Utils.hpp"

using namespace std;

namespace PolyhedraLibrary{
    
    BuildPolyhedra::BuildPolyhedra(const int& Schlafli_p, const int& Schlafli_q)
    {
        p = Schlafli_p;
        q = Schlafli_q;

        NumFaces = (4 * q) / ((2*p) - (p*q) + 2*q); // determines the number of faces using p and q 
        NumEdges = (p * NumFaces) / 2;
        NumVertices = (p * NumFaces) / q;

        // Reserves the exact amount of memory for the differents IDs
        polyhedron.IdVertices.resize(NumVertices);
        for(unsigned int i = 0; i < polyhedron.IdVertices.size(); i++){
            polyhedron.IdVertices[i] = i;
        }
        polyhedron.IdEdges.reserve(NumEdges);
        polyhedron.IdFaces.reserve(NumFaces);

        polyhedron.ListVertPolyhedra.reserve(NumVertices);
        polyhedron.ListEdgePolyhedra.reserve(NumEdges);
        polyhedron.ListFacePolyhedra.reserve(NumFaces);

        // Initialize all the Matrices 
        
        CoordVertices = Eigen::MatrixXd(3, NumVertices);
        ExtremaEdges = Eigen::MatrixXi(2, NumEdges);
        MatrEdgeVertices = Eigen::MatrixXi::Constant(NumVertices, NumVertices, -1);
        ListEdgeFaces = Eigen::MatrixXi(p, NumFaces);
        ListVertFaces = Eigen::MatrixXi(p, NumFaces);
        ListAdjacentFaces = Eigen::MatrixXi(p, NumFaces);
    }

    BuildPolyhedra::BuildPolyhedra(GEOPolyhedron& tesselatedpolyhedron)
    {
        polyhedron = tesselatedpolyhedron;
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
                    polyhedron.lengthEdge = Length_edge;
                    polyhedron.CoordVertices << 0, -0.942809041582063, 0.471404520791031, 0.471404520791031,
                                                0, 0, -0.816496580927726, 0.816496580927726,
                                                1, -0.333333333333333, -0.333333333333333, -0.333333333333333;
                    break;
                case 4:
                    cout << "Your Polyhedron is a Octahedron with:\n";
                    Length_edge = sqrt(2);
                    polyhedron.lengthEdge = Length_edge;
                    polyhedron.CoordVertices << 0, 0, 0, 0, 1, -1,
                                                0, 0, 1, -1, 0, 0,
                                                1, -1, 0, 0, 0, 0;
                    break;
                case 5:
                    cout << "Your Polyhedron is a Icosahedron with: \n";
                    Length_edge = 4 / sqrt(10 + 2*sqrt(5));
                    polyhedron.lengthEdge = Length_edge; 
                    polyhedron.CoordVertices << 0, 0.894427190999916, 0.276393202250021, 0.723606797749979, -0.276393202250021, 0, -0.894427190999916, -0.276393202250021, -0.723606797749979, 0.276393202250021, 0.723606797749979, -0.723606797749979,
                                                0, 0, 0.85065080835204, 0.525731112119134, 0.85065080835204, 0, 0, -0.85065080835204, -0.525731112119134, -0.85065080835204, -0.525731112119133, 0.525731112119134,
                                                1, 0.447213595499958, 0.447213595499958, -0.447213595499958, -0.447213595499958, -1, -0.447213595499958, -0.447213595499958, 0.447213595499958, 0.447213595499957, -0.447213595499958, 0.447213595499958;   
                    break;
                }
                break;
            case 4:
                if (q == 3)
                {
                    cout << "Your Polyhedron is a Cube with: \n";
                    Length_edge = 2 / sqrt(3);
                    polyhedron.lengthEdge = Length_edge;
                }
                break;
            case 5:
                if (q == 3)
                {
                    cout << "Your Polyhedron is a Dodecahedron with: \n";
                    Length_edge = 4 / (sqrt(3) * (1 + sqrt(5)));
                    polyhedron.lengthEdge = Length_edge;
                }
                break;
            }
            cout << NumVertices << " Vertices\n" <<
                    NumEdges << " Edges\n" <<
                    NumFaces << " Faces\n" << endl;
            FillStructPolyhedra();
        }
        else
        {
            cout << "This program cannot handle your polyhedron."  << endl;
            cout << "This program only works with platonic polyhedra."  << endl;
        }
    }

    void BuildPolyhedra::PointsPolyhedra(Eigen::MatrixXd& CoordVertices)
    {   
        polyhedron.CoordVertices = CoordVertices;
        Length_edge = 2 * sqrt(6) / 3; // TODO we need to call the Length Edge inside PointsPolyhedra
        FillStructPolyhedra();
    }

    void BuildPolyhedra::NumberEdges()
    {
        // Per questa funzione ci servono "lenghtEdge", "NumEdges" e il poliedro da cui partire e su cui salvare
       
        /* We didn't find any sequentiality in the ids of the edges, so we decided to find 
        them using their length (which stays always the same)*/
        double lengthEdgeSquared = Length_edge * Length_edge;
        
        /* We need to keep track of how many edges we've found, in order not to find repeating edges: */
        int edgeIndexFound = 0;

        /* We need to find the edges that start from each vertex (except for the last one, 
        because that would be a certain useless iteration: we'll have already found all of the edges 
        that have the last vertex as an extrema) */
        for(int firstVertexIndex = 0; firstVertexIndex < NumVertices - 1; firstVertexIndex++)
        {
            /* Proceed only if all the edges have not been numbered yet */
            if(edgeIndexFound <= NumEdges)
            {
                /* We'll check every other vertex of the polyhedron (for which we don't have already 
                found all edges) in order to find the ones with the exact distance from the vertex 
                with index "firsteVertexIndex" */
                for(int secondVertexIndex = firstVertexIndex + 1; secondVertexIndex < NumVertices; secondVertexIndex++)
                {
                    /* We'll use a function we have implemented in "Utils.cpp" in order to find the 
                    distance squared between the two vertices: */
                    double distanceSquared = distanceSquaredBetween(polyhedron, firstVertexIndex, secondVertexIndex);

                    // cout << "distance edge:" << abs(distanceSquared - lengthEdgeSquared) << endl;
                    /* When the two vertices have the correct distance squared between them we save them as an 
                    edge of the polyhedron (the tolerance was set arbitrarily after some trial and error) */
                    if(abs(distanceSquared - lengthEdgeSquared) < 5e-15)
                    {
                        ExtremaEdges(0, edgeIndexFound) = firstVertexIndex;
                        ExtremaEdges(1, edgeIndexFound) = secondVertexIndex;

                        /* We also save the index of the edge inside the matrix "MatrEdgeVertices": 
                        we'll use it in order to find the adjacent vertices for each vertex and 
                        the faces of the polyhedron */
                        MatrEdgeVertices(firstVertexIndex, secondVertexIndex) = edgeIndexFound;
                        MatrEdgeVertices(secondVertexIndex, firstVertexIndex) = edgeIndexFound;

                        /* Now that we've found an edge we can go on to the next edge: */
                        edgeIndexFound++;
                    }
                }
            }
        }
        
        // cout << "ExtremaEdges: " << endl << ExtremaEdges << endl;
        // cout << "MatrEdgeVertices: " << endl << MatrEdgeVertices << endl;
    }

    void BuildPolyhedra::NumberFaces()
    {
        int faceIndex = 0;

        vector<array<int, 3>> vecVertFaces; // This vector stores unique triangles (faces) as sorted arrays of 3 vertices
        vecVertFaces.reserve(NumFaces);

        vector<vector<int>> adjacencyList = polyhedron.AdjacencyList();
        
        for(int vertex = 0; vertex < NumVertices; vertex++)
        { 
            if (faceIndex < NumFaces) // Proceed only if all the faces have not been numbered yet
            {
                for(auto& vertexToCheck1 : adjacencyList[vertex])
                {
                    // cout << "vertexToCheck1: " << vertexToCheck1 << endl;
                    for(int& vertexToCheck2 : adjacencyList[vertex])
                    {
                        // cout << "vertexToCheck2: " << vertexToCheck2 << endl;
                        
                        // Check all three vertices are distinct
                        if(vertex != vertexToCheck1 && vertex != vertexToCheck2 && vertexToCheck1 != vertexToCheck2)
                        {                    
                            int& edgeIdToAdd = MatrEdgeVertices(vertexToCheck1, vertexToCheck2);     
                            if (edgeIdToAdd >= 0) // Proceed only if there is an edge that connects the two vertices
                            {
                                array<int, 3> sortedVertFace = {vertex, vertexToCheck1, vertexToCheck2};
                                sort(sortedVertFace.begin(), sortedVertFace.end()); // Sorting avoids counting multiple times the same triangles with different vertex ordering

                                // Check if the sorted triangle is already in the vector

                                if(find(vecVertFaces.begin(), vecVertFaces.end(), sortedVertFace) == vecVertFaces.end())
                                {
                                    vecVertFaces.push_back(sortedVertFace);

                                    // Find the edge IDs between the three vertices
                                    int& e1 = MatrEdgeVertices(vertex, vertexToCheck1);
                                    int& e2 = MatrEdgeVertices(vertexToCheck1, vertexToCheck2);
                                    int& e3 = MatrEdgeVertices(vertexToCheck2, vertex);

                                    array<int, 3> edgesInFace = {e1, e2, e3}; 
                                    array<int, 3> verticesInFace = {vertex, vertexToCheck1, vertexToCheck2};

                                    // cout << "Triangolo trovato: (" << vertex << ", " << vertexToCheck1 << ", " << vertexToCheck2 << ")" << endl;

                                    // Check face orientation consistency
                                    if (ExtremaEdges(1, e1) == ExtremaEdges(0, e2)) // e1.end == e2.origin
                                    {                                            
                                        ListVertFaces(0, faceIndex) = verticesInFace[0];
                                        ListVertFaces(1, faceIndex) = verticesInFace[1];
                                        ListVertFaces(2, faceIndex) = verticesInFace[2];

                                        ListEdgeFaces(0, faceIndex) = edgesInFace[0];
                                        ListEdgeFaces(1, faceIndex) = edgesInFace[1];
                                        ListEdgeFaces(2, faceIndex) = edgesInFace[2];
                                        
                                        faceIndex++; // Passing to the next face only if we saved a face during this iteration
                                    }
                                }  
                            } 
                        }  
                        else
                            continue;                         
                    }
                }   
            }
            else
                break; 
        } 
        // Stampa finale per controllo
        // cout << "ListVertFaces: " << endl << ListVertFaces << endl;
        // cout << "ListEdgeFaces: " << endl << ListEdgeFaces << endl;
    }

    void BuildPolyhedra::FillStructPolyhedra()
    {        
        NumberEdges();

        NumberFaces();

        polyhedron.FindAdjacentFaces();
    }

    void BuildPolyhedra::Cell0Ds()
    {
        ofstream file("../PolygonalData/Cell0Ds.txt"); // the program should be launched inside Debug or Release folders
        
        if (file.fail()) 
        {
            std::cerr << "Error opening file\n";
            return;
        }

        file << "Id,X,Y,Z\n";
        for (int i = 0; i < NumVertices; i++)
        {
            file << i << "," << 
            CoordVertices(0, i) << "," << 
            CoordVertices(1, i) << "," << 
            CoordVertices(2, i) << "\n";
        }

        file.close();
    }

    void BuildPolyhedra::Cell1Ds()
    {   
        ofstream file("../PolygonalData/Cell1Ds.txt"); // the program should be launched inside Debug or Release folders
        
        if (file.fail()) 
        {
            std::cerr << "Error opening file\n";
            return;
        }

        file << "Id,Origin,End\n";
        for (int i = 0; i < NumEdges; i++)
        {
            file << i << "," << ExtremaEdges(0, i) << "," << 
            ExtremaEdges(1, i) << "\n";
            
            polyhedron.IdEdges.push_back(i);
        }

        file.close();
    }

    void BuildPolyhedra::Cell2Ds()
    {   
        ofstream file("../PolygonalData/Cell2Ds.txt"); // the program needs to be launched inside Debug or Release folders
        
        if (file.fail()) 
        {
            std::cerr << "Error opening file\n";
            return;
        }

        file << "Id,NumVertices,Vertices,NumEdges,Edges\n";
        for (int i = 0; i < NumFaces; i++)
        {
            file << i << "," << p;
            for (int j = 0; j < p; j++)
            {
                file << "," << ListVertFaces(j, i);
            }
            
            file << "," << p;
            for (int j = 0; j < p; j++)
            {
                file << "," << ListEdgeFaces(j, i);
            }
            file << "\n";

            polyhedron.IdFaces.push_back(i);
        }

        file.close();
    }

    void BuildPolyhedra::Cell3Ds()
    {   
        ofstream file("../PolygonalData/Cell3Ds.txt"); // the program should be launched inside Debug or Release folders

        if (file.fail()) 
        {
            std::cerr << "Error opening file\n";
            return;
        }
        
        file << "IdVertices:";
        for (int i = 0; i < NumVertices; i++)
        {
            file << "V" << polyhedron.IdVertices[i] << ",";
        }

        file  << "\n" << "IdEdges:";
        for (int j = 0; j < NumEdges; j++)
        {
            file << "E" << polyhedron.IdEdges[j] << ",";
        }

        file  << "\n" << "IdEdges:";
        for (int k = 0; k < NumFaces; k++)
        {
            file << "F" << polyhedron.IdFaces[k] << ",";
        }

        // file << "Id,NumVertices,Vertices,NumEdges,Edges,NumFaces,Faces\n";
        // for (int i = 0; i < allPolyhedra.size(); i++)
        // {
        //     file << i << "," << allPolyhedra[i].NumVertices;
        //     for (int j = 0; j < allPolyhedra[i].NumVertices; j++)
        //     {
        //         int v = allPolyhedra[i].IdVertices[j];
        //         file << "," << "V" << v;
        //         allPolyhedra[i].ListVertPolyhedra.push_back(v);
        //         cout << v << endl;
        //     }
            
            // file << "," << allPolyhedra[i].NumEdges;
            // std::cout << "NumEdges: " << allPolyhedra[i].NumEdges << std::endl;
            // std::cout << "IdEdges.size() = " << allPolyhedra[i].IdEdges.size() << std::endl;
            // for(int k :allPolyhedra[i].IdEdges)
            //     cout << k << endl;
            // for (int j = 0; j < allPolyhedra[i].NumEdges; j++)
            // {
            //     int e = allPolyhedra[i].IdEdges[j];
            //     cout << e << endl;
            //     file << "," << "E" << e;
            //     allPolyhedra[i].ListEdgePolyhedra.push_back(e);
            //     cout << j << endl;
            // }
            // std::cout << "IdFaces.size() = " << allPolyhedra[i].IdFaces.size() << std::endl;
            // file << "," << allPolyhedra[i].NumFaces;
            // for (int j = 0; j < allPolyhedra[i].NumFaces; j++)
            // {
            //     int f = allPolyhedra[i].IdFaces[j];
            //     file << "," << "F" << f;
            //     allPolyhedra[i].ListFacePolyhedra.push_back(f);
            //     cout << j << endl;
            // }
            file << "\n";



        file.close();
    }

    void BuildPolyhedra::CreateCells() 
    {
        Cell0Ds();

        Cell1Ds();

        Cell2Ds();

        Cell3Ds();
    }

    GEOPolyhedron BuildPolyhedra::GetPolyhedron()
    {
        return polyhedron;
    }
    
};
