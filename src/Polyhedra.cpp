#include "Polyhedra.hpp"

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
        it all over again with a "switch"*/
        int verticesWithLessFaces = 0;
        switch(q)
        {
        case 3:
            cout << "Your Polyhedron is a Tetrahedron. " << endl;
            verticesWithLessFaces = 4;
            break;
        case 4:
            cout << "Your Polyhedron is a Octahedron. " << endl;
            verticesWithLessFaces = 6;
            break;
        case 5:
            cout << "Your Polyhedron is a Icosahedron. " << endl;
            verticesWithLessFaces = 20;
            break;
        }

        ListFacesWithVertex.reserve(NumVertices);
        /* We need to find for each vertex the faces that have it */
        for(int vertexIndex = 0; vertexIndex < NumVertices; vertexIndex++){
            /* The first "verticesWithLessFaces" vertices only belong to a number "q" of faces,
            so we reserve the exact number of memory space for them */
            if(vertexIndex < verticesWithLessFaces){
                ListFacesWithVertex[vertexIndex].reserve(q);
            }
            /* The other vertices belong to 6 faces, so we reserve the exact number of 
            memory space for them */
            else{
                ListFacesWithVertex[vertexIndex].reserve(6);
            }
            
            /* At each iteration we save how many faces containing the vertex with index "vertexIndex" we've 
            found inside the variable "faceWithVertex" */
            // Non sevre con questo tipo di struttura dati
            int faceWithVertex = 0;
            /* Now we check whther the faces of the matrix "ListVertFaces" contain "vertexIndex" */
            for(int faceIndex = 0; faceIndex < NumFaces; faceIndex++)
            {
                const auto& face = ListVertFaces.col(faceIndex);
                cout << "face: \n" << face << endl;
                if(find(face.begin(), face.end(), vertexIndex) != face.end())
                {
                    ListFacesWithVertex[vertexIndex].push_back(faceIndex);
                    cout << "ListFacesWithVertex(" << faceWithVertex << ", " << vertexIndex << "): " << faceIndex << endl;
                    faceWithVertex++;
                    cout << "faceWithVertex: " << faceWithVertex << endl;
                }
            }
        }
        cout << "ListFacesWithVertex: \n";
        for(int i = 0; i < NumVertices; i++){
            for(unsigned int j = 0; j < ListFacesWithVertex[i].size(); j++){
                cout << ListFacesWithVertex[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }




}