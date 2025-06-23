#include <vector>
#include <iomanip>
#include "Sources.hpp"
#include "UCDUtilities.hpp"

using namespace std;

namespace PolyhedraLibrary
{
    void GEOPolyhedron::CreateStartingPolyhedron()
    {
        NumFaces = (4 * q) / ((2 * p) - (p * q) + 2 * q); // determines the number of faces using p and q 
        NumEdges = (p * NumFaces) / 2;
        NumVertices = (p * NumFaces) / q;

        // Reserves the exact amount of memory for the differents IDs
        IdVertices.resize(NumVertices);
        IdEdges.reserve(NumEdges);
        IdFaces.reserve(NumFaces);

        // Initialize all the Matrices 
        CoordVertices = Eigen::MatrixXd(3, NumVertices);
        ExtremaEdges = Eigen::MatrixXi(2, NumEdges);
        MatrEdgeVertices = Eigen::MatrixXi::Constant(NumVertices, NumVertices, -1);
        ListVertFaces = Eigen::MatrixXi(p, NumFaces);
        ListEdgeFaces = Eigen::MatrixXi(p, NumFaces);
        ListAdjacentFaces = Eigen::MatrixXi(p, NumFaces);
        
        switch (q)
        {
            case 3:
                lengthEdge = 2 * sqrt(6) / 3;
                CoordVertices << 0, -0.942809041582063, 0.471404520791031, 0.471404520791031,
                                0, 0, -0.816496580927726, 0.816496580927726,
                                1, -0.333333333333333, -0.333333333333333, -0.333333333333333;
                break;
            case 4:
                lengthEdge = sqrt(2);
                CoordVertices << 0, 0, 0, 0, 1, -1,
                                0, 0, 1, -1, 0, 0,
                                1, -1, 0, 0, 0, 0;
                break;
            case 5:
                lengthEdge = 4 / sqrt(10 + 2 * sqrt(5)); 
                CoordVertices << 0, 0.894427190999916, 0.276393202250021, 0.723606797749979, -0.276393202250021, 0, -0.894427190999916, -0.276393202250021, -0.723606797749979, 0.276393202250021, 0.723606797749979, -0.723606797749979,
                                0, 0, 0.85065080835204, 0.525731112119134, 0.85065080835204, 0, 0, -0.85065080835204, -0.525731112119134, -0.85065080835204, -0.525731112119133, 0.525731112119134,
                                1, 0.447213595499958, 0.447213595499958, -0.447213595499958, -0.447213595499958, -1, -0.447213595499958, -0.447213595499958, 0.447213595499958, 0.447213595499957, -0.447213595499958, 0.447213595499958;   
                break;
        }


        // Finding the edges of the polyhedron
       
        /* We didn't find any sequentiality in the ids of the edges, so we decided to find 
        them using their length (which stays always the same) until the typeIItessellation */
        
        /* We need to keep track of how many edges we've found, in order not to waste any 
        computational power. Until now we've found none, so we'll pass an integer equal 
        to 0 to the function "FindEdges" (this function will then increase this number 
        until it will be equal to "NumEdges"). Moreover, we'll initialise a nullVec that 
        will be used for the function "FindEdges" and "FindFaces" */
        int edgesFound = 0;
        vector<int> nullVec = {};

        FindEdges(nullVec, edgesFound);


        // Finding the faces of the polyhedron

        /* We need to keep track of how many edges we've found, in order not to waste any 
        computational power. Until now we've found none, so we'll pass an integer equal 
        to 0 to the function "FindFaces" (this function will then increase this number 
        until it will be equal to "NumFaces"). */
        int facesFound = 0;

        /* The function needs a structure where the unique faces of the polyhedron are stored, 
        so we initialise it as empty in order to pass it to the function*/
        vector<array<int, 3>> vecVertFaces;
        vecVertFaces.reserve(NumFaces);

        /* Now we can call the function with all the arguments needed */
        FindFaces(nullVec, facesFound, vecVertFaces, p);
    }

    void GEOPolyhedron::FindEdges(vector<int>& verticesOnFace, int& edgesFound)
    {
        /* Let's initialise the numbers of "verticesToCheck" with the total number of vertices 
        of the polyhedron. If we're interested on just one of its faces, then we can set the size of 
        "verticesOnFace" as the numbers of "verticesToCheck" */
        int verticesToCheck = NumVertices;
        if(verticesOnFace.size() > 0){
            verticesToCheck = int(verticesOnFace.size());
        }
        
        /* Let's start iterating on each vertex of the polyhedron: if we're interested on just one of its 
        faces, then we'll use the "verticesOnFace" vector and its elements */
        for(int vertexId1 = 0; vertexId1 < verticesToCheck - 1; vertexId1++)
        {
            /* Proceed only if all the edges have not been numbered yet */
            if(edgesFound < NumEdges)
            {
                /* Let's rename the index of the vertex for code readability */
                int firstVertexIndex = vertexId1;
                if(verticesOnFace.size() > 0){
                    firstVertexIndex = verticesOnFace[vertexId1];
                }

                /* We'll check every other vertex of the polyhedron (or of the current face) in order to find the ones 
                with the exact distance from the vertex with index "firstVertexIndex" */
                for(int vertexId2 = vertexId1 + 1; vertexId2 < verticesToCheck; vertexId2++)
                {
                    int secondVertexIndex = vertexId2;
                    if(verticesOnFace.size() > 0){
                        secondVertexIndex = verticesOnFace[vertexId2];
                    }

                    /* We'll use a function we have implemented in order to find the 
                    distance squared between the two vertices: */
                    double distanceSquared = DistanceSquaredBetween(firstVertexIndex, secondVertexIndex);

                    /* When the two vertices have the correct distance squared between them we could save them 
                    as an edge of the polyhedron if they have not been saved yet in our data structures 
                    (the tolerance chosen was set arbitrarily after some trial and error) */
                    if(abs(distanceSquared - lengthEdge * lengthEdge) < 5e-15 
                        && MatrEdgeVertices(firstVertexIndex, secondVertexIndex) < 0)
                    {
                        /* Let's save the two vertices ids in the matrix "newExtremaEdges" at the column "edgesFound": */
                        ExtremaEdges(0, edgesFound) = firstVertexIndex;
                        ExtremaEdges(1, edgesFound) = secondVertexIndex;

                        /* Let's also save the index of the edge inside the matrix "MatrEdgeVertices": 
                        we'll use it in order to find the faces of the polyhedron */
                        MatrEdgeVertices(firstVertexIndex, secondVertexIndex) = edgesFound;
                        MatrEdgeVertices(secondVertexIndex, firstVertexIndex) = edgesFound;
    
                        /* Now that we've found an edge we can go on to the next edge increasing "edgesFound"*/
                        edgesFound++;
                    }
                }
            }
        }
    }

    void GEOPolyhedron::FindFaces(vector<int>& verticesOnFace, int& facesFound, vector<array<int, 3>>& vecVertFaces, int& numAdjacentVertices)
    {

        /* Let's find the adjacencyList for each vertex */
        vector<vector<int>> adjacencyList = AdjacencyList(verticesOnFace, numAdjacentVertices);

        /* Let's initialise the numbers of "verticesToCheck" with the total number of vertices 
        of the polyhedron. If we're interested on just one of its faces, then we can set the size of 
        "verticesOnFace" as the numbers of "verticesToCheck" */
        int verticesToCheck = NumVertices;
        if(verticesOnFace.size() > 0){
            verticesToCheck = int(verticesOnFace.size());
        }

        /* Let's start iterating on each vertex of the polyhedron. If we're interested on just one of its 
        faces, then we'll use the "verticesOnFace" vector: at each iteration we'll access its next element */
        for(int vertexId = 0; vertexId < verticesToCheck; vertexId++)
        { 
            int vertex = vertexId;
            if(verticesOnFace.size() > 0)
            {
                vertex = verticesOnFace[vertexId];
            }

            /* Let's go on with the algorithm only if we didn't find all of the faces of the polyhedron yet 
            and there are some adjacent vertices to "vertex". If that's not the case, we go on with the external 
            "for" cycle until it finds a vertex with some adjacent vertices or until it simply ends */
            if(facesFound < NumFaces && adjacencyList[vertex].size() > 0)
            {
                /* Let's iterate on the other adjacent vertices of "vertex" */
                for(int& vertexToCheck1 : adjacencyList[vertex])
                {
                    /* Let's again iterate on the other adjacent vertices of "vertex" */
                    for(int& vertexToCheck2 : adjacencyList[vertex])
                    {
                        /* Let's go on with the algorithm only if the 3 vertices are distinct */
                        if(vertex != vertexToCheck1 && vertex != vertexToCheck2 && vertexToCheck1 != vertexToCheck2)
                        {                       
                            /* Let's rename the edge ids between the 3 vertices for code readability */
                            int& e1 = MatrEdgeVertices(vertex, vertexToCheck1);
                            int& e2 = MatrEdgeVertices(vertexToCheck1, vertexToCheck2);
                            int& e3 = MatrEdgeVertices(vertexToCheck2, vertex);

                            /* The first vertex is of course connected to its adjacent vertices with edge "e1" 
                            and "e3", so we need to check whether the other two vertices are connected 
                            between them. If that's the case, we go on with the algorithm */
                            if(e2 >= 0)
                            {
                                /* Let's initialise and sort an array containing the vertices of the face 
                                in order to avoid counting multiple times the same triangles with 
                                different vertex ordering */
                                array<int, 3> sortedVertFace = {vertex, vertexToCheck1, vertexToCheck2};
                                sort(sortedVertFace.begin(), sortedVertFace.end()); 
                                
                                /* Let's go on with the algorithm only if the sorted triangle isn't in the vector yet */
                                if(find(vecVertFaces.begin(), vecVertFaces.end(), sortedVertFace) == vecVertFaces.end())
                                {
                                    /* Let's add the face to our list of unique faces */
                                    vecVertFaces.push_back(sortedVertFace);
                                          
                                    /* Let's save the new face in our data structures */
                                    ListVertFaces(0, facesFound) = sortedVertFace[0];
                                    ListVertFaces(1, facesFound) = sortedVertFace[1];
                                    ListVertFaces(2, facesFound) = sortedVertFace[2];

                                    ListEdgeFaces(0, facesFound) = e1;
                                    ListEdgeFaces(1, facesFound) = e2;
                                    ListEdgeFaces(2, facesFound) = e3;
                                    
                                    /* Now that we've found one, let's increase the number of faces found */
                                    facesFound++; 
                                }  
                            } 
                        }                     
                    }
                }   
            }
        }
    }

    vector<vector<int>> GEOPolyhedron::AdjacencyList(vector<int>& verticesOnFace, int& numAdjacentVertices)
    {
        /* Let's initialise the structure "adjacencyList" with "NumVertices" memory spaces
        in order to avoid segmentation faults: we don't know which vertices ids are on each face, 
        so we don't want to access the element with id 19 if the structure has only 10 elements */
        vector<vector<int>> adjacencyList(NumVertices);

        /* Let's initialise the numbers of "verticesToCheck" with the total number of vertices 
        of the polyhedron. If we're interested on just one of its faces, then we can set the size of 
        "verticesOnFace" as the numbers of "verticesToCheck"*/
        int verticesToCheck = NumVertices;
        if(verticesOnFace.size() > 0)
        {
            verticesToCheck = int(verticesOnFace.size());
        }

        /* Let's start iterating on each vertex of the polyhedron. If we're interested on just one of its 
        faces, then we'll use the "verticesOnFace" vector: at each iteration we'll access its next element */
        for(int vertexId = 0; vertexId < verticesToCheck; vertexId++)
        {
            int vertex = vertexId;
            if(verticesOnFace.size() > 0)
            {
                vertex = verticesOnFace[vertexId];
            }

            /* Now that we accessed the correct vertex id, we can reserve the maximum memory space its 
            adjacency list will need, which is tha maximum number of adjacent vertices for a vertex in 
            the polyhedron, information stored inside "numAdjacentVertices" */
            adjacencyList[vertex].reserve(numAdjacentVertices);

            /* Let's start iterating on each possible adjacent vertex of the polyhedron. If we're interested 
            on just one of its faces, then we'll use the "verticesOnFace" vector: at each iteration 
            we'll access its next element */
            for(int adjVertId = 0; adjVertId < verticesToCheck; adjVertId++)
            {
                int adjVert = adjVertId;
                if(verticesOnFace.size() > 0)
                {
                    adjVert = verticesOnFace[adjVertId];
                }

                /* If the edge exists, which means "MatrEdgeVertices" has an element greater or equal to 0 in 
                position ("vertex", "adjVert", then "adjVert" is an adjacent vertex of "vertex" 
                and we can save it inside its adjacency list */
                if(MatrEdgeVertices(vertex, adjVert) >= 0)
                {
                    adjacencyList[vertex].push_back(adjVert);
                }
            }
        }
        /* After we've created the adjacency list for each vertex, we can return the structure */
        return adjacencyList;
    }

    void GEOPolyhedron::ExportPolyhedron(Path& minimumPath)
    {
        /* Let's initialise two vectors with elements of type "Gedim::UCDProperty<double>" as empty vectors */
        vector<Gedim::UCDProperty<double>> VerticesProperties = {};
        vector<Gedim::UCDProperty<double>> EdgesProperties = {};

        /* If "minimumPath" is a proper "Path" object, which means it has a "VerticesShortPath" 
        and an "EdgesShortPath" that are not empty, we save their data inside the 
        "VerticesProperties" and "EdgesProperties" vectors */
        if(minimumPath.VerticesShortPath.size() > 0 && minimumPath.EdgesShortPath.size() > 0)
        {
            /* First, we need to resize the vector in order to store the property as an element */
            VerticesProperties.resize(1);

            /* Then, we define the element of "VerticesProperties" giving it: 
                - a property "label" (used only inside ParaView to access the property); 
                - an "unit label" (used only inside ParaView to access the property);
                - the "size" of the property, which is the number of elements described by the property;
                - the "number of components" of the property, which is 1 in our case;
                - the "data" of the property, stored inside "VerticesShortPath". */
            VerticesProperties[0] = {
                "ShortPath",
                "colour",
                unsigned(NumVertices),
                1,
                minimumPath.VerticesShortPath.data()
            };

            /* We also do the same thing as above for "EdgesProperties" using the data 
            inside "EdgesShortPath" */
            EdgesProperties.resize(1);
            EdgesProperties[0] = {
                "ShortPath",
                "colour",
                unsigned(NumEdges),
                1,
                minimumPath.EdgesShortPath.data()
            };
        }

        /* Let's create some markers for the data structures of the polyhedron in order 
        to see them colourful inside ParaView */
        Eigen::VectorXi VerticesMarkers(NumVertices);
        for(int i = 0; i < NumVertices; i++)
        {
            VerticesMarkers[i] = 0;
        }
        
        Eigen::VectorXi EdgesMarkers(NumEdges);
        for(int i = 0; i < NumEdges; i++)
        {
            EdgesMarkers[i] = NumEdges - i;
        }

        Eigen::VectorXi FacesMarkers(NumFaces);
        for(int i = 0; i < NumFaces; i++)
        {
            FacesMarkers[i] = i;
        }

        /* Let's export the vertices and edges of the polyhedron */
        Gedim::UCDUtilities exporter;
        exporter.ExportPoints("../PolygonalData/Cell0Ds.inp",
                               CoordVertices,
                               VerticesProperties,
                               VerticesMarkers);

        exporter.ExportSegments("../PolygonalData/Cell1Ds.inp",
                                 CoordVertices,
                                 ExtremaEdges,
                                 VerticesProperties,
                                 EdgesProperties,
                                 EdgesMarkers);


        /* Creating a vector of vectors starting from ListVertFaces in order 
        to use Mr. Vicini's code contained in "UCDUtilities.hpp" as intended*/
        vector<vector<unsigned int>> FacesVertices;
        FacesVertices.resize(NumFaces);

        for(int i = 0; i < NumFaces; i++)
        {
            FacesVertices[i].resize(3);
            
            /* Saving the vertices of the face with index "i" inside the vector with index "i" */
            FacesVertices[i][0] = ListVertFaces(0, i);
            FacesVertices[i][1] = ListVertFaces(1, i);
            FacesVertices[i][2] = ListVertFaces(2, i);
        }
        
        /* Lastly, let's export the faces of the polyhedron */
        exporter.ExportPolygons("../PolygonalData/Cell2Ds.inp",
                                  CoordVertices,
                                  FacesVertices,
                                  {},
                                  {},
                                  FacesMarkers);
        
        /* Let's export on the files .txt as requested by the project using the function "CellXDs" */
        Cell0Ds();
        Cell1Ds();
        Cell2Ds();
        Cell3Ds();
    }

    void GEOPolyhedron::Cell0Ds()
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
            scientific << setprecision(16) <<
            CoordVertices(0, i) << "," << 
            CoordVertices(1, i) << "," << 
            CoordVertices(2, i) << "\n";

            IdVertices.push_back(i);
        }

        file.close();
    }

    void GEOPolyhedron::Cell1Ds()
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
            
            IdEdges.push_back(i);
        }

        file.close();
    }

    void GEOPolyhedron::Cell2Ds()
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

            IdFaces.push_back(i);
        }

        file.close();
    }

    void GEOPolyhedron::Cell3Ds()
    {   
        ofstream file("../PolygonalData/Cell3Ds.txt"); // the program should be launched inside Debug or Release folders

        if (file.fail()) 
        {
            std::cerr << "Error opening file\n";
            return;
        }
        
        file << "IdVertices: ";
        for (int i = 0; i < NumVertices; i++)
        {
            if(i == NumVertices - 1)
            {
                /* For aesthetic reasons, the last vertex doesn't need the comma afterwards */
                file << "V" << IdVertices[i];
            }
            else
            {
                file << "V" << IdVertices[i] << ",";
            }
        }
        file << endl;

        file  << "\n" << "IdEdges: ";
        for (int j = 0; j < NumEdges; j++)
        {
            if(j == NumEdges - 1)
            {
                file << "E" << IdEdges[j];
            }
            else
            {
                file << "E" << IdEdges[j] << ",";
            }
        }
        file << endl;

        file  << "\n" << "IdFaces: ";
        for (int k = 0; k < NumFaces; k++)
        {
            if(k == NumFaces - 1)
            {
                file << "F" << IdFaces[k];
            }
            else
            {
                file << "F" << IdFaces[k] << ",";
            }  
        }

            file << "\n";

        file.close();
    }

    double GEOPolyhedron::DistanceSquaredBetween(int& idPoint1, int& idPoint2)
    {
        /* Let's rename the variables we'll use for code readability */
        double& point1XCoord = CoordVertices(0, idPoint1);
        double& point1YCoord = CoordVertices(1, idPoint1);
        double& point1ZCoord = CoordVertices(2, idPoint1);

        double& point2XCoord = CoordVertices(0, idPoint2);
        double& point2YCoord = CoordVertices(1, idPoint2);
        double& point2ZCoord = CoordVertices(2, idPoint2);

        /* Let's compute the distance squared as the sum of all the differences between the coordinates of 
        the points squared (we don't use the "pow" function because we were taught by Ms. Teora that for 
        little exponents the product is better) */
        double distanceSquared = (point1XCoord - point2XCoord) * (point1XCoord - point2XCoord) +
                                 (point1YCoord - point2YCoord) * (point1YCoord - point2YCoord) + 
                                 (point1ZCoord - point2ZCoord) * (point1ZCoord - point2ZCoord);

        return distanceSquared;
    }

}