#include <vector>
#include "Polyhedra.hpp"
#include "UCDUtilities.hpp"

using namespace std;

namespace PolyhedraLibrary
{

    /* FindAdjacentFaces is a function we'll need in order to avoid duplicating the edges and vertices 
    in the tessellations. It finds the adjacent faces "i" for each face "j" and saves them in the matrix 
    "ListAdjacentFaces" (which is part of the "polyhedron" class) at position (i,j)*/
    void GEOPolyhedron::FindAdjacentFaces()
    {
        /* We need to find the adjacent faces for each face: 
        we'll do it by finding two faces of the matrix "ListVertFace" that contain 
        the same two vertices ids */
        for(int faceIndex = 0; faceIndex < NumFaces; faceIndex ++)
        {
            // Saving the vertices we'll need to find with an alias for code readability
            int& vertexToFind1 = ListVertFaces(0, faceIndex);
            int& vertexToFind2 = ListVertFaces(1, faceIndex);
            int& vertexToFind3 = ListVertFaces(2, faceIndex);
            
            /* We need to save the id of the plausible adjacent faces we're checking somewhere 
            in order to save it inside the matrix "ListAdjacentFaces" */
            int adjacentFacesFound = 0;
            for(int faceWeAreChecking = 0; faceWeAreChecking < NumFaces; faceWeAreChecking++)
            {
                const auto& face = ListVertFaces.col(faceWeAreChecking);
                /* We need to skip the face which we're looking the adjacent faces for */
                if(faceWeAreChecking == faceIndex){
                    continue;
                }
                /* Checking that we haven't found all of the adjacent faces for the face with id "faceIndex" */
                else if(adjacentFacesFound < 3)
                {
                    // Non funziona ancora per il poliedro tassellato: indagare
                    if(find(face.begin(), face.end(), vertexToFind1) != face.end())
                    {
                        /* In this case we've found the first vertex inside one face of the 
                        matrix "ListVertFace". We need to find another vertex */
                        if(find(face.begin(), face.end(), vertexToFind2) != face.end() ||
                           find(face.begin(), face.end(), vertexToFind3) != face.end())
                        {
                            /* In this case we've found one adjacent face for the face with id "faceIndex". 
                            We can save it inside the matrix "ListAdjacentFaces" */
                            ListAdjacentFaces(adjacentFacesFound, faceIndex) = faceWeAreChecking;
                            adjacentFacesFound++;
                            continue;
                        }
                    }
                    else if(find(face.begin(), face.end(), vertexToFind2) != face.end())
                    {
                        /* In this case we've found the second vertex inside one face of the 
                        matrix "ListVertFace". We need to find another vertex */
                        if(find(face.begin(), face.end(), vertexToFind1) != face.end() ||
                           find(face.begin(), face.end(), vertexToFind3) != face.end())
                        {
                            ListAdjacentFaces(adjacentFacesFound, faceIndex) = faceWeAreChecking;
                            adjacentFacesFound++;
                            continue;
                        }
                    }
                    else if(find(face.begin(), face.end(), vertexToFind3) != face.end())
                    {
                        // cout << "Found once the third vertex " << endl;
                        /* In this case we've found the third vertex inside one face of the 
                        matrix "ListVertFace". We need to find another vertex */
                        if(find(face.begin(), face.end(), vertexToFind1) != face.end() ||
                           find(face.begin(), face.end(), vertexToFind2) != face.end())
                        {
                            // cout << "Found an adjacent face " << endl;
                            ListAdjacentFaces(adjacentFacesFound, faceIndex) = faceWeAreChecking;
                            adjacentFacesFound++;
                            continue;
                        }
                    }
                }
            }
        }

        // Stampa finale per controllo
        // cout << "ListVertFaces: " << endl << ListVertFaces << endl;
        // cout << "ListAdjacentFaces: " << endl << ListAdjacentFaces << endl;

    }

    vector<vector<int>> GEOPolyhedron::AdjacencyList(vector<int>& verticesOnFace, int& numAdjacentVertices)
    {
        /* Let's initialise the structure "adjacencyList" with "NumVertices" memory spaces
        in order to avoid segmentation faults: we don't know which vertices ids are on each face, 
        so we don't want to access the element with id 19 if the structure ha only 10 elements */
        vector<vector<int>> adjacencyList(NumVertices);

        /* Let's initialise the numbers of vertices to check with the total number of vertices 
        of the polyhedron. If we're interested on just one of its faces, then we can set the size of 
        "verticesOnFace" as the numbers of vertices to check */
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
            the polyhedron, number stored inside "numAdjacentVertices" */
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

                /* We'll go on only if the two vertices are distinct, otherwise we would check 
                a useless edge, wasting computational power */

                // è anche vero che questa cosa succede solo una votla e le nostre strutture 
                // non memorizzano lati sbagliati, quindi a una sola iterazione questo if è utile, 
                // altrimenti è spreco di controllo a mio avviso
                if(vertex != adjVert) 
                {        
                    /* Let's rename the edge we'll check for code readability */            
                    int& edgeIdToCheck = MatrEdgeVertices(vertex, adjVert);

                    /* If the edge exists, then "adjVert" is an adjacent vertex of "vertex" 
                    and we can save it inside its adjacency list */
                    if(edgeIdToCheck >= 0)
                    {
                        adjacencyList[vertex].push_back(adjVert);
                    }
                }
            }
        }
        /* After we've created the adjacency list for each vertex, we can return the structure */
        return adjacencyList;
    }

    void GEOPolyhedron::FindFaces(vector<int>& verticesOnFace, int& newFacesFound, vector<array<int, 3>>& vecVertFaces, int& numAdjacentVertices)
    {
        /* Let's find the adjacencyList for each vertex */
        vector<vector<int>> adjacencyList = AdjacencyList(verticesOnFace, numAdjacentVertices);
        
        /* Let's initialise the numbers of vertices to check with the total number of vertices 
        of the polyhedron. If we're interested on just one of its faces, then we can set the size of 
        "verticesOnFace" as the numbers of vertices to check */
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

            /* Let's go on with the algorithm only if we didn't find alla of the faces of the polyhedron yet 
            or if tthere aren't any adjacent vertices to "vertex". In those cases, we simply go on with 
            the external "for" cycle until it ends */
            if(newFacesFound < NumFaces && adjacencyList[vertex].size() > 0) // Proceed only if all the faces have not been numbered yet
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

                            /* The first vertex is of course connected to its adjacent vertices, 
                            so we need to check whether the other two are connected between them. 
                            If that's the case, then w ego on with the algorithm */
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
                                    
                                    /* Just for aesthetic reasons, we decided to sort the edges of the face. */
                                    // Ovviamente possiamo anche togliere questa riga per risparmiare 
                                    // complessità computazionale
                                    array<int, 3> sortedEdgeFace = {e1, e2, e3};
                                    sort(sortedEdgeFace.begin(), sortedEdgeFace.end());
                                          
                                    /* Let's save the new face in our data structures */
                                    ListVertFaces(0, newFacesFound) = sortedVertFace[0];
                                    ListVertFaces(1, newFacesFound) = sortedVertFace[1];
                                    ListVertFaces(2, newFacesFound) = sortedVertFace[2];

                                    ListEdgeFaces(0, newFacesFound) = sortedEdgeFace[0];
                                    ListEdgeFaces(1, newFacesFound) = sortedEdgeFace[1];
                                    ListEdgeFaces(2, newFacesFound) = sortedEdgeFace[2];
                                    
                                    /* Let's increase the number of faces found now that we've found one */
                                    newFacesFound++; 
                                }  
                            } 
                        }                     
                    }
                }   
            }
        }
    }

    // Per il momento inutile
    void GEOPolyhedron::FindFacesWithVertex()
    {
        /* First, we need to understand how many vertices the polyhedron not tessellated has.
        We updated the value of NumVertices during the tessellation, so we need to understand 
        it all over again with a "switch" */
        int verticesWithLessFaces = 0;
        switch(q)
        {
            case 3:
                verticesWithLessFaces = 4;
                break;
            case 4:
                verticesWithLessFaces = 6;
                break;
            case 5:
                verticesWithLessFaces = 20;
                break;
        }

        /* We need to use a resize otherwise the program aborts with exception "bad_alloc" */
        ListFacesWithVertex.resize(NumVertices);
        /* We need to find for each vertex the faces to which the vertex belongs to */
        for(int vertexIndex = 0; vertexIndex < NumVertices; vertexIndex++)
        {
            /* The first "verticesWithLessFaces" vertices only belong to a number "q" of faces,
            so we reserve the exact number of memory space for them */
            if(vertexIndex < verticesWithLessFaces)
            {
                ListFacesWithVertex[vertexIndex].reserve(q);
            }
            /* The other vertices belong to 6 faces, so we reserve the exact number of 
            memory space for them */
            else
            {
                ListFacesWithVertex[vertexIndex].reserve(6);
            }

            /* Now we check whether the faces of the matrix "ListVertFaces" contain "vertexIndex" */
            for(unsigned int faceIndex = 0; faceIndex < unsigned(NumFaces); faceIndex++)
            {
                const auto& face = ListVertFaces.col(faceIndex);
                if(find(face.begin(), face.end(), vertexIndex) != face.end())
                {
                    ListFacesWithVertex[vertexIndex].push_back(faceIndex);
                }
            }
        }
    }

    void GEOPolyhedron::ExportPolyhedronWithoutFaces()
    {
        /* In order to export the polyhedra, we need to create the markers for the vertices, 
        the edges and the faces. We decided them arbitrarily in order to make them pop to the eye.
        The vertices have all marker 0 because they are not difficult to separate from one another */
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

        /* Then, we can export all of the structures passing the arguments needed to the functions */
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
    }

    void GEOPolyhedron::ExportPolyhedron(Path& minimumPath)
    {
        std::vector<Gedim::UCDProperty<double>> VerticesProperties = {};
        std::vector<Gedim::UCDProperty<double>> EdgesProperties = {};
        if(minimumPath.VerticesShortPath.size() > 0 && minimumPath.EdgesShortPath.size() > 0)
        {
            VerticesProperties.resize(1);
            VerticesProperties[0] = {
                "ShortPath",
                "colour",
                unsigned(NumVertices),
                1,
                minimumPath.VerticesShortPath.data()
            };

            EdgesProperties.resize(1);
            EdgesProperties[0] = {
                "ShortPath",
                "colour",
                unsigned(NumEdges),
                1,
                minimumPath.EdgesShortPath.data()
            };
        }

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
        to use Mr. Vicini's code (UCDUtilities.hpp) */
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
        exporter.ExportPolygons("../PolygonalData/Cell2Ds.inp",
                                  CoordVertices,
                                  FacesVertices,
                                  {},
                                  {},
                                  FacesMarkers);
    }

}