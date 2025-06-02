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
# include "UCDUtilities.hpp"

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

        // Initialize all the Matrices 
        
        CoordVertices = Eigen::MatrixXd(3, NumVertices);
        ExtremaEdges = Eigen::MatrixXi(2, NumEdges);
        MatrEdgeVertices = Eigen::MatrixXi::Constant(NumVertices, NumVertices, -1);
        ListEdgeFaces = Eigen::MatrixXi(p, NumFaces);
        ListVertFaces = Eigen::MatrixXi(p, NumFaces);
        ListAdjacentFaces = Eigen::MatrixXi(p, NumFaces);
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
        
        cout << "ExtremaEdges: " << endl << ExtremaEdges << endl;
        cout << "MatrEdgeVertices: " << endl << MatrEdgeVertices << endl;
    }
 
    vector<vector<int>> BuildPolyhedra::AdjacencyList()
    {
        vector<vector<int>> adjacencyList(NumVertices);

        for(int vertex = 0; vertex < NumVertices; vertex++)
        {
            for(int adjVert = 0; adjVert < NumVertices; adjVert++)
            {
                if (vertex != adjVert) 
                {                    
                    int& edgeIdToCheck = MatrEdgeVertices(vertex, adjVert);
                    if (edgeIdToCheck >= 0)
                    {
                        adjacencyList[vertex].push_back(adjVert);
                    }
                }
            }
        }
        return adjacencyList;
    }

    void BuildPolyhedra::NumberFaces()
    {
        int faceIndex = 0;

        vector<array<int, 3>> vecVertFaces; // This vector stores unique triangles (faces) as sorted arrays of 3 vertices
        vecVertFaces.reserve(NumFaces);

        vector<vector<int>> adjacencyList = AdjacencyList();
        
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
        cout << "ListVertFaces: " << endl << ListVertFaces << endl;
        cout << "ListEdgeFaces: " << endl << ListEdgeFaces << endl;
    }

    /* FindAdjacentFaces is a function we'll need in order to avoid duplicating the edges and vertices 
    in the tessellations. It finds the adjacent faces "i" for each face "j" and saves them in the matrix 
    "ListAdjacentFaces" (which is part of the "polyhedron" class) at position (i,j)*/
    void BuildPolyhedra::FindAdjacentFaces(){
        /* We need to find the adjacent faces for each face: 
        we'll do it by finding two columns of the matrix "ListVertFace" that contain 
        the same two vertices ids */
        for(int faceIndex = 0; faceIndex < NumFaces; faceIndex ++)
        {
            // Saving the vertices we'll need to find with an alias for code readability
            int& vertexToFind1 = ListVertFaces(0, faceIndex);
            int& vertexToFind2 = ListVertFaces(1, faceIndex);
            int& vertexToFind3 = ListVertFaces(2, faceIndex);
            
            /* We need to save the id of the face we're checking somewhere in order to save it 
            inside the matrix "ListAdjacentFaces" */
            int faceWeAreChecking = 0;
            int adjacentFacesFound = 0;
            for(auto column : ListVertFaces.colwise()){
                
                /* We need to skip the face which we're looking the adjacent faces for */
                if(faceWeAreChecking == faceIndex){
                    faceWeAreChecking++;
                    continue;
                }
                /* Checking that we haven't found all of the adjacent faces for the face with id "faceIndex" */
                else if(adjacentFacesFound < q)
                {
                    if(find(column.begin(), column.end(), vertexToFind1) != column.end())
                    {
                        /* In this case we've found the first vertex inside one column of the 
                        matrix "ListVertFace". We need to find another vertex */
                        if(find(column.begin(), column.end(), vertexToFind2) != column.end() ||
                           find(column.begin(), column.end(), vertexToFind3) != column.end())
                        {
                            /* In this case we've found one adjacent face for the face with id "faceIndex". 
                            We can save it inside the matrix "ListAdjacentFaces" */
                            ListAdjacentFaces(adjacentFacesFound, faceIndex) = faceWeAreChecking;
                            adjacentFacesFound++;
                        }
                    }

                    else if(find(column.begin(), column.end(), vertexToFind2) != column.end())
                    {
                        if(find(column.begin(), column.end(), vertexToFind1) != column.end() ||
                           find(column.begin(), column.end(), vertexToFind3) != column.end())
                        {
                            ListAdjacentFaces(adjacentFacesFound, faceIndex) = faceWeAreChecking;
                            adjacentFacesFound++;
                        }
                    }

                    else if(find(column.begin(), column.end(), vertexToFind3) != column.end())
                    {
                        if(find(column.begin(), column.end(), vertexToFind1) != column.end() ||
                           find(column.begin(), column.end(), vertexToFind2) != column.end())
                        {
                            ListAdjacentFaces(adjacentFacesFound, faceIndex) = faceWeAreChecking;
                            adjacentFacesFound++;
                        }
                    }

                }
                else
                {
                    /* When we've found all of the adjacent faces for the face with id "faceIndex", 
                    we can go on looking for the adjacent faces of face with the next "faceIndex" */
                    break;
                }
                faceWeAreChecking++;
            }
        }

        // Stampa finale per controllo
        cout << "ListAdjacentFaces: " << endl << ListAdjacentFaces << endl;

    }

    void BuildPolyhedra::FillStructPolyhedra()
    {        
        NumberEdges();

        NumberFaces();

        FindAdjacentFaces();
    }

    void BuildPolyhedra::Cell0Ds()
    {
        ofstream file("../PolygonalData/Cell0Ds.txt"); // the program should be launched inside Debug or Release folders
        
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
        
        file << "IdVertices:\n";
        for (int i = 0; i < NumVertices; i++)
        {
            file << "V" << polyhedron.IdVertices[i] << "\n";
        }

        for (int j = 0; j < NumEdges; j++)
        {
            file << "E" << polyhedron.IdEdges[j] << "\n";
        }

        for (int k = 0; k < NumFaces; k++)
        {
            file << "F" << polyhedron.IdFaces[k] << "\n";
        }

        file.close();
    }

    void BuildPolyhedra::CreateCells() 
    {
        Cell0Ds();

        Cell1Ds();

        Cell2Ds();

        Cell3Ds();
    }

    void BuildPolyhedra::ExportPolyhedra()
    {
        Eigen::VectorXi VerticesMarkers(NumVertices);
        for(int i = 0; i < NumVertices; i++)
        {
            VerticesMarkers[i] = 0;
        }
        
        Eigen::VectorXi EdgesMarkers(NumEdges);
        for(int i = 0; i < NumEdges; i++)
        {
            EdgesMarkers[i] = 5;
        }

        Gedim::UCDUtilities utilities;
        utilities.ExportPoints("../PolygonalData/Cell0Ds.inp",
                               CoordVertices,
                               {},
                               VerticesMarkers);

        utilities.ExportSegments("../PolygonalData/Cell1Ds.inp",
                                 CoordVertices,
                                 ExtremaEdges,
                                 {},
                                 {},
                                 EdgesMarkers);
        
        /* Creating a vector of vectors starting from ListVertFaces in order 
        to use Mr. Vicini's code (UCDUtilities.hpp) */
        
        vector<vector<unsigned int>> FacesVertices;
        FacesVertices.resize(NumFaces);

        for(int i = 0; i < NumFaces; i++)
        {
            FacesVertices[i].resize(3);
            
            // Salvo i vertici di ciascuna faccia con indice "i" all'interno del vettore con indice "i"
            FacesVertices[i][0] = ListVertFaces(0, i);
            FacesVertices[i][1] = ListVertFaces(1, i);
            FacesVertices[i][2] = ListVertFaces(2, i);
        }

        Eigen::VectorXi FacesMarkers(NumFaces);
        for(int i = 0; i < NumFaces; i++)
        {
            FacesMarkers[i] = i;
        }

        utilities.ExportPolygons("../PolygonalData/Cell2Ds.inp",
                                  CoordVertices,
                                  FacesVertices,
                                  {},
                                  {},
                                  FacesMarkers);
    
    }

    GEOPolyhedron BuildPolyhedra::GetPolyhedron()
    {
        return polyhedron;
    }
    
};
