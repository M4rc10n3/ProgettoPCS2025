#include "Polyhedra.hpp"

using namespace std;

namespace PolyhedraLibrary
{

    /* FindAdjacentFaces is a function we'll need in order to avoid duplicating the edges and vertices 
    in the tessellations. It finds the adjacent faces "i" for each face "j" and saves them in the matrix 
    "ListAdjacentFaces" (which is part of the "polyhedron" class) at position (i,j)*/
    void GEOPolyhedron::FindAdjacentFaces(){
        /* Just to make the code more generalised */


        /* We need to find the adjacent faces for each face: 
        we'll do it by finding two columns of the matrix "ListVertFace" that contain 
        the same two vertices ids */
        
        cout << "ListAdjacentFaces: \n" << endl << ListAdjacentFaces << endl;

        for(int faceIndex = 0; faceIndex < NumFaces; faceIndex ++)
        {
            // Saving the vertices we'll need to find with an alias for code readability
            int& vertexToFind1 = ListVertFaces(0, faceIndex);
            int& vertexToFind2 = ListVertFaces(1, faceIndex);
            int& vertexToFind3 = ListVertFaces(2, faceIndex);
            
            /* We need to save the id of the plausible adjacent faces we're checking somewhere 
            in order to save it inside the matrix "ListAdjacentFaces" */
            int faceWeAreChecking = 0;
            int adjacentFacesFound = 0;
            for(auto column : ListVertFaces.colwise())
            {
                /* We need to skip the face which we're looking the adjacent faces for */
                if(faceWeAreChecking == faceIndex){
                    faceWeAreChecking++;
                    continue;
                }
                /* Checking that we haven't found all of the adjacent faces for the face with id "faceIndex" */
                else if(adjacentFacesFound < 3)
                {
                    // Non funziona ancora per il poliedro tassellato: indagare
                    if(find(column.begin(), column.end(), vertexToFind1) != column.end())
                    {
                        cout << "Found once the first vertex " << endl;
                        /* In this case we've found the first vertex inside one column of the 
                        matrix "ListVertFace". We need to find another vertex */
                        if(find(column.begin(), column.end(), vertexToFind2) != column.end() ||
                           find(column.begin(), column.end(), vertexToFind3) != column.end())
                        {
                            cout << "Found an adjacent face " << endl;
                            /* In this case we've found one adjacent face for the face with id "faceIndex". 
                            We can save it inside the matrix "ListAdjacentFaces" */
                            ListAdjacentFaces(adjacentFacesFound, faceIndex) = faceWeAreChecking;
                            adjacentFacesFound++;
                            continue;
                        }
                    }
                    else if(find(column.begin(), column.end(), vertexToFind2) != column.end())
                    {
                        cout << "Found once the second vertex " << endl;
                        /* In this case we've found the second vertex inside one column of the 
                        matrix "ListVertFace". We need to find another vertex */
                        if(find(column.begin(), column.end(), vertexToFind1) != column.end() ||
                           find(column.begin(), column.end(), vertexToFind3) != column.end())
                        {
                            cout << "Found an adjacent face " << endl;
                            ListAdjacentFaces(adjacentFacesFound, faceIndex) = faceWeAreChecking;
                            adjacentFacesFound++;
                            continue;
                        }
                    }
                    else if(find(column.begin(), column.end(), vertexToFind3) != column.end())
                    {
                        cout << "Found once the third vertex " << endl;
                        /* In this case we've found the third vertex inside one column of the 
                        matrix "ListVertFace". We need to find another vertex */
                        if(find(column.begin(), column.end(), vertexToFind1) != column.end() ||
                           find(column.begin(), column.end(), vertexToFind2) != column.end())
                        {
                            cout << "Found an adjacent face " << endl;
                            ListAdjacentFaces(adjacentFacesFound, faceIndex) = faceWeAreChecking;
                            adjacentFacesFound++;
                            continue;
                        }
                    }

                }
                
                /* Whenever we don't find any vertex in common bewtween the faces or we've found 
                an adjacent face, we can go on to the next face to check */
                faceWeAreChecking++;

            }
            cout << "for face: " << faceIndex << ", adjacentFacesFound: " << adjacentFacesFound << endl;
        }

        // Stampa finale per controllo
        cout << "ListVertFaces: \n" << endl << ListVertFaces << endl;
        cout << "ListAdjacentFaces: \n" << endl << ListAdjacentFaces << endl;

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

}