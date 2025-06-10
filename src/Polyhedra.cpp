#include "Polyhedra.hpp"
# include "UCDUtilities.hpp"

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

    vector<vector<int>> GEOPolyhedron::AdjacencyList()
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

    void GEOPolyhedron::ExportPolyhedron()
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

        Eigen::VectorXi FacesMarkers(NumFaces);
        for(int i = 0; i < NumFaces; i++)
        {
            FacesMarkers[i] = i;
        }

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

        /* Lastly, we can export all of the structures passing the arguments needed to the functions */
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

        utilities.ExportPolygons("../PolygonalData/Cell2Ds.inp",
                                  CoordVertices,
                                  FacesVertices,
                                  {},
                                  {},
                                  FacesMarkers);
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

}