/* File containing the body of the functions we'll use */
# include <iomanip>
# include "Utils.hpp"
# include "Polyhedra.hpp"
# include "BuildPolyhedra.hpp"
# include "../ExportParaview/UCDUtilities.hpp"
# include <vector>
# include <queue>

using namespace PolyhedraLibrary;
using namespace std;
using namespace Eigen;

Eigen::Vector3d FindBarycenter(GEOPolyhedron& polyhedron, Eigen::Vector3i& VertFace)
{
    Eigen::Vector3d barycenter_coordinates = Eigen::Vector3d::Zero(3); // initialyze a vector for the final coordinates
    double Coord_x = 0.0;
    double Coord_y = 0.0;
    double Coord_z = 0.0;

    // Finding the coordinates of each vertex 
    for (int i = 0; i < VertFace.size(); i++)
    {
        Coord_x += polyhedron.CoordVertices(0, VertFace(i));
        Coord_y += polyhedron.CoordVertices(1, VertFace(i));
        Coord_z += polyhedron.CoordVertices(2, VertFace(i));
    }

    barycenter_coordinates = {Coord_x/3.0, Coord_y/3.0, Coord_z/3.0};

    return barycenter_coordinates;
}

vector<int> BFS(const vector<vector<int>>& adjList, const int& v1, const int& v2, const int& numVert, const double& lengthEdge)
{
    queue<int> q;
    vector<bool> reached(numVert); // boolean vector to save visited vertices
    vector<int> predecessor(numVert); // vector to save the predecessor of each vertex in order to reconstruct the minimum path
    for(int i = 0; i < numVert; i++) // initialize reached and predecessor vectors
    {
        reached[i] = false;
        predecessor[i] = -1;
    }
    q.push(v1);
    reached[v1] = true;
    while(!q.empty())
    {
        int u = q.front();
        q.pop();
        for(int w : adjList[u])
        {
            if(!reached[w]) // check if the predecessor vertex hasn't been visited yet
            {
                reached[w] = true; // mark as visited
                predecessor[w] = u; // save the predecessor of w
                q.push(w); // add to the queue
            }    
            if(w == v2)
            {
                // Reconstruct the path from v2 back to v1  (using the predecessor of each vertex and finally reversing the created vector)
                vector<int> minPath;
                int v = v2;
                while(v != -1) 
                {
                    minPath.push_back(v);
                    v = predecessor[v];                  
                }
                reverse(minPath.begin(), minPath.end());
                
                // Output the path and calculate total length
                cout << "The minimum path (using BFS algorithm) is: ";
                for(int i : minPath)
                    cout << i << " ";
                cout << endl;
                cout << "The minimum path is composed of " << minPath.size() - 1 << " edges" << endl;
                cout << "The minimum path length is " << (minPath.size() - 1) * lengthEdge << endl;  

                return minPath; // return the list of vertices in the minimum path            
            }
        }
    }
    return {}; // if no path was found, return an empty vector  
}

vector<int> Dijkstra(const vector<vector<int>>& adjList, const int& v1, const int& v2, const int& numVert, Eigen::MatrixXd& matrWeights)
{
    vector<int> predecessor(numVert); // vector to save the predecessor of each vertex
    vector<double> distance(numVert); // vector to store the current known shortest distance from v1 to each vertex
    const double inf = 1e9; // use a large number to represent "infinity"
    for(int i = 0; i < numVert; i++) 
    {
        predecessor[i] = -1; // Initialize all predecessors to -1
        distance[i] = inf; // initialize all distances to "infinity"
    }
    // Set the source vertex v1
    predecessor[v1] = v1;
    distance[v1] = 0.0;

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq; // std::greater<int> makes the max priority queue act as a min priority queue
    for(int i = 0; i < numVert; i++) 
    {
        pq.push({distance[i], i}); // initialize the priority queue
    }

    while(!pq.empty())
    {
        // Dequeue the node with the smallest distance
        auto [dist_u, u] = pq.top();
        pq.pop();

        // If we already found a better distance to u, skip it
        if(dist_u > distance[u])
            continue;
            
        for(int w : adjList[u])
        {
            if(distance[w] > distance[u] + matrWeights(u,w)) // check if going through u gives a shorter path to w
            {
                distance[w] = distance[u] + matrWeights(u,w); // update the distance to w with the one passing through u
                predecessor[w] = u; // set u as predecessor of w
                pq.push({distance[w], w}); // add the updated distance into the priority queue
            }  
        }
    }

    if (predecessor[v2] == -1) 
    {
        cout << "No path found from " << v1 << " to " << v2 << endl;
        return {}; // if no path was found, return an empty vector
    }

    // Reconstruct the path from v2 back to v1 (using the predecessor of each vertex and finally reversing the created vector)
    vector<int> minPath;
    int v = v2;
    while(v != v1) 
    {
        minPath.push_back(v);
        v = predecessor[v];                  
    }
    minPath.push_back(v1);
    reverse(minPath.begin(), minPath.end());
    
    // Output the path and calculate total length
    double lengthPath = 0.0;
    cout << "The minimum path (using Dijkstra algorithm) is: " << minPath[0];
    for(int i = 1; i < minPath.size(); i++)
    {
        cout << " " << minPath[i];
        lengthPath += matrWeights(minPath[i-1], minPath[i]);
    }                        
    cout << endl;
    cout << "The minimum path is composed of " << minPath.size() - 1 << " edges" << endl;
    cout << "The minimum path length is " << lengthPath << endl;  

    return minPath; // return the list of vertices in the minimum path                  
}

void MinimumPath(const vector<int>& minPath, const Eigen::MatrixXi& MatrEdgeVertices, const int& numVert, const int& numEdge, vector<int>& vertShortPath, vector<int>& edgeShortPath)
{
    vertShortPath.resize(numVert);
    edgeShortPath.resize(numEdge);
    for(int i = 0; i < numVert; i++) // initialize vertShortPath vectors
    {
        vertShortPath[i] = 0;
    }
    for(int i = 0; i < numEdge; i++) // initialize edgeShortPath vector
    {
        edgeShortPath[i] = 0;
    }

    // set the property ShortPath = 1 to the vertices and the edges that compose the minimum path
    for(int i : minPath)
    {
        vertShortPath[i] = 1; 
    }
    cout << endl;

    for(int i = 1; i < minPath.size(); i++)
    {
        const int& idEdge = MatrEdgeVertices(minPath[i-1], minPath[i]);
        if(idEdge > -1)
        {
            edgeShortPath[idEdge] = 1;
        }
    }
}

void Dualise(GEOPolyhedron& polyhedron, const int& Schlafli_p, const int& Schlafli_q)
{
    /* Non ci serve una funzione che dualizzi un poliedro di base, poiché basta che partiamo proprio 
    dal duale, ovvero, se dovessimo avere bisogno del duale del cubo, partiamo direttamente dall'ottaedro.
    Ciò che ci serve è il duale del poliedro tassellato */
    
    Eigen::MatrixXd CoordVertices(3, polyhedron.NumFaces);
    BuildPolyhedra constructor(Schlafli_q, Schlafli_p); // p, q are switched because it's the dual polyhedron

    cout << "\n DualPolyhedron \n" << endl;

    for (int i = 0; i < polyhedron.NumFaces; i++)
    {
        Eigen::Vector3i dual_vertex  = polyhedron.ListVertFaces.col(i);
        Eigen::Vector3d barycenter_coordinates = FindBarycenter(polyhedron, dual_vertex);
        CoordVertices.col(i) = barycenter_coordinates.normalized(); // project the point into the 3D sphere
    }

    constructor.PointsPolyhedra(CoordVertices);
    constructor.CreateCells();
    constructor.ExportPolyhedra();
}

GEOPolyhedron TypeITessellation(GEOPolyhedron& polyhedron, int& numberDivisions)
{
    GEOPolyhedron tessellatedPolyhedron;

    
    /* We'll need the number of triangles for some resizing, so let's calculate it using 
    the formulas given in "PCS_Project.pdf" by Miss. Teora */
    const int numberTrianglesForFace = numberDivisions * numberDivisions; // T = b^2
    int& numberNewVertices = tessellatedPolyhedron.NumVertices;
    int& numberNewEdges = tessellatedPolyhedron.NumEdges;
    int& numberNewFaces = tessellatedPolyhedron.NumFaces;
    numberNewVertices = 0; // V = 2 * T + 2
    numberNewEdges = 0; // E = 6 * T
    numberNewFaces = 0; // F = 4 * T

    int& q = polyhedron.q;
    /* The formulas for the new values of the polyhedron depend on the value of the Schläfli number q,
    so let's divide the different cases: */
    switch (q)
    {
        case 3:
            numberNewVertices = 2 * numberTrianglesForFace + 2; // V = 2 * T + 2
            numberNewEdges = 6 * numberTrianglesForFace; // E = 6 * T
            numberNewFaces = 4 * numberTrianglesForFace; // F = 4 * T
            break;
        case 4:
            numberNewVertices = 4 * numberTrianglesForFace + 2; // V = 4 * T + 2
            numberNewEdges = 12 * numberTrianglesForFace; // E = 12 * T
            numberNewFaces = 8 * numberTrianglesForFace; // F = 8 * T
            break;
        case 5:
            numberNewVertices = 10 * numberTrianglesForFace + 2; // V = 10 * T + 2
            numberNewEdges = 30 * numberTrianglesForFace; // E = 30 * T
            numberNewFaces = 20 * numberTrianglesForFace; // F = 20 * T
            break;
    }
    
    /* We'll use the old polyhedron data structures to save the new vertices, edges and faces, so
    let's rename them for code readability and resize them (conservatively) using the values 
    calculated above. */

    const int& oldNumFaces = polyhedron.NumFaces;
    const int& oldNumEdges = polyhedron.NumEdges;
    const int& oldNumVertices = polyhedron.NumVertices;
    const double& oldLengthEdge = polyhedron.lengthEdge;

    Eigen::MatrixXd& oldCoordVertices = polyhedron.CoordVertices;
    Eigen::MatrixXd& newCoordVertices = tessellatedPolyhedron.CoordVertices;
    newCoordVertices.resize(3, numberNewVertices);
    /* CoordVertices will be divided like so:
        - the first columns (they will be a quantity equal to oldNumVertices) will be for the 
        coordinates of the vertices of the old polyhedron;
        - the next columns will be for the coordinates of the vertices on each edge: there will be 
        (numberDivisions - 1) columns for each edge;
        - the last columns of the matrix will be for the coordinates of the inner vertices of each 
        face which we'll no longer access, so we don't care of their order as a function of 
        other values */    
    newCoordVertices.leftCols(oldNumVertices) = oldCoordVertices.leftCols(oldNumVertices);

    /* We'll use the matrix "ExtremaEdges" for finding the vertices of the new polyhedron, 
    but we won't update it with the new edges found. We will substitute it at the end of 
    the edge-finding algorithm with the matrix "newExtremaEdges". 
    We'll do the same thing with "MatrEdgeVertices", which will be substitued by 
    the matrix "newMatrEdgeVertices" */
    Eigen::MatrixXi& oldExtremaEdges = polyhedron.ExtremaEdges;
    Eigen::MatrixXi& newExtremaEdges = tessellatedPolyhedron.ExtremaEdges;
    newExtremaEdges.resize(2, numberNewEdges);

    // Eigen::MatrixXi& oldMatrEdgeVertices = polyhedron.MatrEdgeVertices;
    Eigen::MatrixXi& newMatrEdgeVertices = tessellatedPolyhedron.MatrEdgeVertices;
    newMatrEdgeVertices = Eigen::MatrixXi::Constant(numberNewVertices, numberNewVertices, -1);

    const int& p = polyhedron.p;
    // Eigen::MatrixXi& oldListVertFaces = polyhedron.ListVertFaces;
    Eigen::MatrixXi& newListVertFaces = tessellatedPolyhedron.ListVertFaces;
    newListVertFaces.resize(p, numberNewFaces);

    Eigen::MatrixXi& oldListEdgeFaces = polyhedron.ListEdgeFaces;
    Eigen::MatrixXi& newListEdgeFaces = tessellatedPolyhedron.ListEdgeFaces;
    newListEdgeFaces.resize(p, numberNewFaces);

    /* We'll need the length of the edge of the new polyhedron in order to find the inner vertices 
    of each face and most importantly all of its edges, so let's compute it: */
    const double newLengthEdge = oldLengthEdge / numberDivisions;
    tessellatedPolyhedron.lengthEdge = newLengthEdge;

    const int numberVerticesOnFace = triangularNumber(numberDivisions);

    /* Let's initialize a vector that will save all of the edges that we have already divided.
    It will v1 with just "-1" as its elements. Then, we'll also need to keep track of its 
    index in order to increase it and to access the right element of the vector. */
    vector<int> edgesDone(oldNumEdges, -1);
    int edgesDoneIndex = 0;

    /* In order not to get it out of scope, we need to initialize here the variable we'll 
    use to keep track of how many inner vertices we have found and saved. This variable 
    will be uesed to access the right column of the matrix "CoordVertices". */
    int innerVerticesSaved = 0;

    /* Same thing with the variable we'll use to keep track of how many edges we have found and saved.
    This variable will be used to access the right column of the matrix "newExtremaEdges" and as an 
    element saved into the matrix "newMatrEdgeVertices". */
    int edgeIndexFound = 0;

    int newFacesFound = 0;

    vector<vector<int>> verticesOnAllFaces;
    verticesOnAllFaces.reserve(oldNumFaces);
    for(int i = 0; i < oldNumFaces; i++){
        verticesOnAllFaces[i].reserve(numberVerticesOnFace);
    }

    /* We need to tesselate each face of the polyhedron given as input, so we need a "for" 
    cycle on the old polyhedron faces: */
    for(int faceIndex = 0; faceIndex < oldNumFaces; faceIndex++)
    {   
        /* Let's create a vector containing the ids of the vertices on each face. 
        We'll use it to find the polyhedron new edges using on each face the same algorithm 
        we used for the original polyhedron. We cannot use it in all of the polyhedron because we 
        would otherwise find other edges inside the polyhedron itself which we don't need. */
        vector<int> verticesOnFace;
        verticesOnFace.reserve(numberVerticesOnFace);

        /* Let's create some vectors that we'll use to save the coordinates of the new vertices on each edge;
        they are Eigen vectors because we'll use the method "LinSpaced" of the same library on them */
        Eigen::VectorXd newXCoordVerticesOnEdge = VectorXd::Zero(numberDivisions + 1);
        Eigen::VectorXd newYCoordVerticesOnEdge = VectorXd::Zero(numberDivisions + 1);
        Eigen::VectorXd newZCoordVerticesOnEdge = VectorXd::Zero(numberDivisions + 1);

        /* In order to tesselate, we need to divide each edge into "numberDivisions" parts,
         so we need a "for" cycle on the edges of each face: */
        for(int edge = 0; edge < p; edge++)
        {
            /* Let's find the index we used in our data structures for the edge of the face: */
            int& edgeIndex = oldListEdgeFaces(edge, faceIndex);
            
            /* Let's save inside "verticesOnFace" the vertices of the edges that are already divided */
            if(find(edgesDone.begin(), edgesDone.end(), edgeIndex) != edgesDone.end()){
                /* First of all, let's save the vertices that are the extrema of the edges */
                for(int extremum = 0; extremum < 2; extremum++){
                    /* Let's rename a variable for code readability */
                    int& edgeExtremum = oldExtremaEdges(extremum, edgeIndex);
                    /* We can save the vertex with a simple "push_back" if we haven't already 
                    saved it before */
                    if(find(verticesOnFace.begin(), verticesOnFace.end(), edgeExtremum) == verticesOnFace.end())
                    {
                        verticesOnFace.push_back(edgeExtremum);
                    }   
                }
                
                /* Then, let's save the vertices that divide the edge into "numberDivisions" parts. 
                They were certainly never saved inside "verticesOnFace" during this iteration of faceIndex.*/
                for(int division = 0; division < numberDivisions - 1; division++){
                    verticesOnFace.push_back(oldNumVertices + 
                                            (numberDivisions - 1) * edgeIndex +
                                            division);
                }

                /* Then, we'll skip the division algorithm for the edges we've already divided */
                continue;
                
            }      
            else
            {
                /* Using the edgeIndex we found above we can access the right vertices 
                indexes inside our data structures and rename them for code readability: */
                int& firstVertexWeAreDividing = oldExtremaEdges(0, edgeIndex);
                int& secondVertexWeAreDividing = oldExtremaEdges(1, edgeIndex);

                /* Let's add the two vertices to the vector "verticesOnFace" only if they are not already 
                an element of it */
                if(find(verticesOnFace.begin(), verticesOnFace.end(), firstVertexWeAreDividing) == verticesOnFace.end())
                {
                    verticesOnFace.push_back(firstVertexWeAreDividing);
                }
                if(find(verticesOnFace.begin(), verticesOnFace.end(), secondVertexWeAreDividing) == verticesOnFace.end())
                {
                    verticesOnFace.push_back(secondVertexWeAreDividing);
                }

                /* Let's rename the coordinates of the two vertices for code readability: */
                double& firstVertexXCoord = oldCoordVertices(0, firstVertexWeAreDividing);
                double& firstVertexYCoord = oldCoordVertices(1, firstVertexWeAreDividing);
                double& firstVertexZCoord = oldCoordVertices(2, firstVertexWeAreDividing);

                double& secondVertexXCoord = oldCoordVertices(0, secondVertexWeAreDividing);
                double& secondVertexYCoord = oldCoordVertices(1, secondVertexWeAreDividing);
                double& secondVertexZCoord = oldCoordVertices(2, secondVertexWeAreDividing);

                /* Calculating the coordinates of the new vertices using the function LinSpaced()
                of the Eigen library */
                newXCoordVerticesOnEdge = VectorXd::LinSpaced(numberDivisions + 1, 
                                                                firstVertexXCoord, 
                                                                secondVertexXCoord);
                newYCoordVerticesOnEdge = VectorXd::LinSpaced(numberDivisions + 1, 
                                                                firstVertexYCoord,
                                                                secondVertexYCoord);
                newZCoordVerticesOnEdge = VectorXd::LinSpaced(numberDivisions + 1, 
                                                                firstVertexZCoord, 
                                                                secondVertexZCoord);

                /* Let's add the coordinates of each new point (indicated by "division") on 
                each edge to the matrix "CoordVertices" as descripted above: */
                for(int division = 0; division < numberDivisions - 1; division++)
                {
                    /* In order to access the proper column, we should ignore:
                        - the vertices of the old polyhedron; 
                        - the vertices that divide the edges before the current one;
                    then we should access the vertex with id "division". 
                    Each of these members is represented in a line in the next sum for code readability: */
                    int newVertexId = oldNumVertices + 
                                      (numberDivisions - 1) * edgeIndex +
                                      division;

                    newCoordVertices(0, newVertexId) = newXCoordVerticesOnEdge(division + 1);
                    newCoordVertices(1, newVertexId) = newYCoordVerticesOnEdge(division + 1);
                    newCoordVertices(2, newVertexId) = newZCoordVerticesOnEdge(division + 1);

                    /* Let's also add the id of the new vertex to the vector "verticesOnFace": */
                    verticesOnFace.push_back(newVertexId);

                }

                /* Now that we have divided an edge, we can save it inside the vector "edgesDone" 
                in order not to divide it again.*/
                edgesDone[edgesDoneIndex] = edgeIndex;
                edgesDoneIndex++;
            }
        }

        /* Now we need to find the coordinates of the inner vertices of each face of the 
        old polyhedron */

        /* Let's find the vertices that have the exact distance we need to create the internal points. 
        For example, for b = 3, we'll search for the vertices on the edges (except for their extrema) 
        that have distance equal to two times the length of the new smaller edge. 
        For b = 4, we'll firstly look for the vertex that has a distance equal to 3 times that of the edge, 
        then we'll save the edge index and search on it for the vertex that has a distance equal to 2 times 
        that of the edge.
        Once we'll have found the two etrema of the inner segment, we'll use the same algorithm as above 
        to find the inner vertices subdividing the segment into the exact number of parts */

        /* We'll always start our search from the vertices on the first edge of the face and we'll look for 
        the vertices on the other edges of the same face */
        int& edgeWeStartFromIndex = oldListEdgeFaces(0, faceIndex);

        /* Let's create the variable that will save the index of the opposing edge to the one with index 
        "edgeWeStartFromIndex" which has the vertex we found with the first search for the vertex with the 
        exact distance we're looking for */
        int minimumDistanceOpposingEdge = -1;

        for(int time = 0; time < numberDivisions - 2; time++)
        /* "time" represents the number of inner segments that each face has */
        {
            /* Let's set a "minimumDistance" we'll use to find the exact vertex we're looking for. 
            Unfortunately, a tolerance wasn't feasible because on some faces the distance from the 
            first vertex and the vertex we're looking for was of the order of e-15 and on some other 
            faces it was of the order of e-16 without folloqing an understandable logic */
            double minimumDistance = newLengthEdge;

            /* Let's create here the variable that save the indexes of the vertices which we'll divide
             into the exact number of parts at each iteration of "time" */
            int minimumDistanceIdVertexWeStartFrom = -1;
            int minimumDistanceIdOpposingVertex = -1;

            /* At the first iteration of "time" we'll look for the edge with the vertex we're looking 
            for and then we'll save it inside "minimumDistanceOpposingEdge" in order to use it afterwards.
            After the second iteration we'll look for the vertex only on the edge with index "minimumDistanceOpposingEdge"
            in order not to create repeating sides.*/
            if(time == 0)
            {
                /* We'll look for the right vertex on both the opposing vertices: */
                for(int opposingEdgeIndex = 1; opposingEdgeIndex < p; opposingEdgeIndex++)
                {
                    /* Let's find the right index of the edge accessing our data structures:  */
                    int& opposingEdge = oldListEdgeFaces(opposingEdgeIndex, faceIndex);
                    
                    /* Now we can look for the right vertex on the edge: */
                    for(int idVertexOnEdge = 0; idVertexOnEdge < (numberDivisions - 1); idVertexOnEdge++)
                    {
                        /* In order to access the proper vertex where we'll v1 from, we should ignore:
                            - the vertices of the old polyhedron;
                            - the vertices we found above on the edges before the current one; 
                        then we should access the vertex with id "time". 
                        Each of these members is represented in a line in the next sum for code readability: */
                        int idVertexWeStartFrom = oldNumVertices + 
                                                  (numberDivisions - 1) * edgeWeStartFromIndex +
                                                  time;
    
                        /* In order to access the proper vertex where we'll end at, we should ignore:
                            - the vertices of the old polyhedron;
                            - the vertices we found above on the edges before the current one; 
                        then we should access the vertex with id "idVertexOnEdge". 
                        Each of these members is represented in a line in the next sum for code readability: */
                        int idOpposingVertex = oldNumVertices + 
                                               (numberDivisions - 1) * opposingEdge +
                                               idVertexOnEdge;
    
                        /* "distance" is the variable contanining the difference between the distance squared 
                        between the vertices and the length of the segment we're looking for */
                        double distance = abs(distanceSquaredBetween(tessellatedPolyhedron, idVertexWeStartFrom, idOpposingVertex) - 
                            newLengthEdge * (numberDivisions - 1 - time) * newLengthEdge * (numberDivisions - 1 - time));
    
                        /* When we found a vertex with a "distance" that's less than that we've already saved, 
                        then we update the value of "minimumDistance", the indexes of the vertices of the segment we 
                        will divide and the index of the edge where we'll look for the vertices after the first 
                        iteration of "time" */
                        if(distance < minimumDistance)
                        {
                            minimumDistance = distance;

                            minimumDistanceIdVertexWeStartFrom = idVertexWeStartFrom;
                            minimumDistanceIdOpposingVertex = idOpposingVertex;

                            minimumDistanceOpposingEdge = opposingEdge;
                        }
                    }
                }
            }
            else
            {
                /* After the first iteration, so for "time" > 0, we already know the index of the opposing edge, 
                which is saved in the variable initialised outside the "for" cycle on the "time" variable */
                int& opposingEdge = minimumDistanceOpposingEdge;
                    
                /* Now we can look for the right vertex on the edge we saved: */
                for(int idVertexOnEdge = 0; idVertexOnEdge < (numberDivisions - 1); idVertexOnEdge++)
                {
                    /* In order to access the proper vertex where we'll v1 from, we should ignore:
                        - the vertices of the old polyhedron;
                        - the vertices we found above on the edges before the current one; 
                    then we should access the vertex with id "time". 
                    Each of these members is represented in a line in the next sum for code readability: */
                    int idVertexWeStartFrom = oldNumVertices + 
                                              (numberDivisions - 1) * edgeWeStartFromIndex +
                                              time;

                    /* In order to access the proper vertex where we'll end at, we should ignore:
                        - the vertices of the old polyhedron;
                        - the vertices we found above on the edges before the current one; 
                    then we should access the vertex with id "idVertexOnEdge". 
                    Each of these members is represented in a line in the next sum for code readability: */
                    int idOpposingVertex = oldNumVertices + 
                                           (numberDivisions - 1) * opposingEdge +
                                           idVertexOnEdge;
                    
                    /* "distance" is the variable contanining the difference between the distance squared 
                    between the vertices and the length of the segment we're looking for */
                    double distance = abs(distanceSquaredBetween(tessellatedPolyhedron, idVertexWeStartFrom, idOpposingVertex) - 
                        newLengthEdge * (numberDivisions - 1 - time) * newLengthEdge * (numberDivisions - 1 - time));

                    /* When we found a vertex with a "distance" that's less than that we've already saved, 
                    then we update the value of "minimumDistance" and the indexes of the vertices of the segment we 
                    will divide (we don't know to update the variable "minimumDistanceOpposingEdge" anymore) */
                    if(distance < minimumDistance){
                            minimumDistance = distance;
                            minimumDistanceIdVertexWeStartFrom = idVertexWeStartFrom;
                            minimumDistanceIdOpposingVertex = idOpposingVertex;
                        }
                }
            }
            /* After we've found the vertices with the minimum "distance" we'll divide them using
            the same algorithm as above (used for the vertices on the edges) in order to find 
            the coordinates of the inner vertices (in this case we need to access the coordinates 
            in the matrix "newCoordVertices" of the new polyhedron)*/
            int& firstVertexWeAreDividing = minimumDistanceIdVertexWeStartFrom;
            int& secondVertexWeAreDividing = minimumDistanceIdOpposingVertex;

            double& firstVertexXCoord = newCoordVertices(0, firstVertexWeAreDividing);
            double& firstVertexYCoord = newCoordVertices(1, firstVertexWeAreDividing);
            double& firstVertexZCoord = newCoordVertices(2, firstVertexWeAreDividing);

            double& secondVertexXCoord = newCoordVertices(0, secondVertexWeAreDividing);
            double& secondVertexYCoord = newCoordVertices(1, secondVertexWeAreDividing);
            double& secondVertexZCoord = newCoordVertices(2, secondVertexWeAreDividing);

            newXCoordVerticesOnEdge = VectorXd::LinSpaced(numberDivisions - time, 
                                                        firstVertexXCoord, 
                                                        secondVertexXCoord);
            newYCoordVerticesOnEdge = VectorXd::LinSpaced(numberDivisions - time, 
                                                        firstVertexYCoord,
                                                        secondVertexYCoord);
            newZCoordVerticesOnEdge = VectorXd::LinSpaced(numberDivisions - time, 
                                                        firstVertexZCoord, 
                                                        secondVertexZCoord);

            /* We need to save the coordinates of the new vertices with a different 
            algorithm compared to the one used above: */
            for(int vertexSaved = 0; vertexSaved < numberDivisions - 2 - time; vertexSaved++)
            {
                /* In order to find the proper index of the new vertex, we should ignore:
                    - the vertices of the old polyhedron;
                    - all of the vertices we found above on the edges of the old polyhedron; 
                then we should access the vertex with id "innerVerticesSaved", variable 
                initialised outside the "for" cycle on the faces of the old polyhedron. 
                Each of these members is represented in a line in the next sum for 
                code readability: */
                int newVertexId = oldNumVertices + 
                                    (numberDivisions - 1) * oldNumEdges + 
                                    innerVerticesSaved;
                newCoordVertices(0, newVertexId) = newXCoordVerticesOnEdge(vertexSaved + 1);
                newCoordVertices(1, newVertexId) = newYCoordVerticesOnEdge(vertexSaved + 1);
                newCoordVertices(2, newVertexId) = newZCoordVerticesOnEdge(vertexSaved + 1);

                /* Now that we've found an inner vertex, we can increase the variable 
                that counts them: */
                innerVerticesSaved++;

                /* Let's also add the id of the new vertex to the vector "verticesOnFace": */
                verticesOnFace.push_back(newVertexId);
            }
        }
       
        /* Now we'll find the polyhedron new edges using a similar algorithm used for the old polyhedra 
        based on the length of the edge. It will be modified because we would also find internal edges 
        that we don't need using the original algorithm . */
    
        /* We need to find the edges that v1 from each vertex of the face (except for the last one, 
        because that would be a certain useless iteration: we'll have already found all of the edges 
        that have the last vertex as an extrema). We will use the "verticesOnFace" vector to find 
        the right index of each vertex in our data structures */
        for(int verticesOnFaceIndex1 = 0; verticesOnFaceIndex1 < numberVerticesOnFace - 1; verticesOnFaceIndex1++)
        {
            /* Proceed only if all the edges have not been numbered yet */
            if(edgeIndexFound < numberNewEdges)
            {
                /* Let's rename the index of the vertex for code readability */
                int& firstVertexIndex = verticesOnFace[verticesOnFaceIndex1];

                /* We'll check every other vertex of the current face in order to find the ones 
                with the exact distance from the vertex with index "firsteVertexIndex" */
                for(int verticesOnFaceIndex2 = verticesOnFaceIndex1 + 1; verticesOnFaceIndex2 < numberVerticesOnFace; verticesOnFaceIndex2++)
                {
                    int& secondVertexIndex = verticesOnFace[verticesOnFaceIndex2];

                    /* We'll use a function we have implemented in order to find the 
                    distance squared between the two vertices: */
                    double distanceSquared = distanceSquaredBetween(tessellatedPolyhedron, firstVertexIndex, secondVertexIndex);

                    /* When the two vertices have the correct distance squared between them we could save them 
                    as an edge of the polyhedron if they have not been saved yet (the tolerance was set arbitrarily 
                    after some trial and error) */
                    if(abs(distanceSquared - newLengthEdge * newLengthEdge) < 5e-15)
                    {
                        /* We need to check whether we have already saved the two vertices as the extrema of 
                        an edge, therefore we need to check whether inside the matrix "newMatrEdgeVertices" 
                        there's an element saved at the position "(firstVertexIndex, secondVertexIndex)" */
                        if(newMatrEdgeVertices(firstVertexIndex, secondVertexIndex) < 0){
                            /* Now that we know the two vertices have not been saved yet, we can save them 
                            in the matrix "newExtremaEdges" at the column "edgeIndexFound": */
                            newExtremaEdges(0, edgeIndexFound) = firstVertexIndex;
                            newExtremaEdges(1, edgeIndexFound) = secondVertexIndex;

                            /* We also save the index of the edge inside the matrix "MatrEdgeVertices": 
                            we'll use it in order to find the adjacent vertices for each vertex and 
                            the faces of the polyhedron */
                            newMatrEdgeVertices(firstVertexIndex, secondVertexIndex) = edgeIndexFound;
                            newMatrEdgeVertices(secondVertexIndex, firstVertexIndex) = edgeIndexFound;
        
                            /* Now that we've found an edge we can go on to the next edge: */
                            edgeIndexFound++;
                        }
                    }
                }
            }
        }
        verticesOnAllFaces.push_back(verticesOnFace);

        /* Mettere qui dentro il prossimo algoritmo è complesso e fa creare strutture in più su 
        ListVertFaces, ListeEdgeFaces e ExtremaEdges, magari poi ci proverò. Infatti, l'algoritmo 
        ha bisogno della matrice MatrEdgeVertices aggiornata, eppure aggiornando le altre 3 strutture 
        tutto l'algoritmo smette di funzionare, perché accediamo a strutture ormai modificate */
    }

    // /* Now that we've found all of the vertices and the edges of the new polyhedron 
    // we can update the data structures of the polyhedron itself */
    // MatrEdgeVertices = newMatrEdgeVertices;
    // ExtremaEdges = newExtremaEdges;

    // MatrEdgeVertices.makeCompressed(); // in tal caso creandola bisogna incrementare tutti gli indici per averla sparsa


    /* Now we'll search for the faces of the polyhedron using the same algorithm used
    for the original polyhedron but using it on each face in order not to find inner 
    faces of the polyhedron */
    for(int faceIndex = 0; faceIndex < oldNumFaces; faceIndex++)
    {   
        /* Let's initialise the vector which stores unique triangles (the faces of the polyhedron) 
        as sorted arrays of 3 vertices. It will store the unique new faces found on the old polyhedron 
        face with index "faceIndex" */
        vector<array<int, 3>> vecVertFaces;
        vecVertFaces.reserve(numberNewFaces / oldNumFaces);

        /* Creating the adjacency list for the vertices. We don't care which vertex has the maximum 
        index on the face, so we need to initialise it using more memory than what I need in order to avoid
        segmentation faults */
        vector<vector<int>> adjacencyList(numberNewVertices);
        /* We need to find the adjacent vertices for each vertex on the face of the old polyhedron */
        for(int vertexId = 0; vertexId < numberVerticesOnFace; vertexId++)
        {
            /* Let's access the right "adjVert" for our data structures index using the 
            structure "verticesOnAllFaces" */
            int& vertex = verticesOnAllFaces[faceIndex][vertexId];
            adjacencyList[vertex].reserve(6);
    
            for(int adjVertId = 0; adjVertId < numberVerticesOnFace; adjVertId++)
            {
                /* Let's access the right "adjVert" for our data structures index using the 
                structure "verticesOnAllFaces" */
                int& adjVert = verticesOnAllFaces[faceIndex][adjVertId];
                /* We don't care about the iteration during which "vertex" and "adjVert" are equal, 
                so we skip it */
                if(vertex != adjVert) 
                {                    
                    /* Let's access the right "edgeIdToCheck" in our data structures */
                    int& edgeIdToCheck = newMatrEdgeVertices(vertex, adjVert);
                    if(edgeIdToCheck >= 0)
                    {
                        /* If the edge exists, then the vertex with index "adjVert" is one of the 
                        adjacent vertices for the vertex with index "vertex" and we can add it to its 
                        vector inside "adjacencyList" */
                        adjacencyList[vertex].push_back(adjVert);
                        
                    }
                }
            }
        }
        
        /* Now we can look for the new faces of the polyhedron v1ing from each of the vertices 
        on the face of the old polyhedron */
        for(int vertexId = 0; vertexId < numberVerticesOnFace; vertexId++)
        {
            /* Let's access the right "vertex" for our data structures index using the 
            structure "verticesOnAllFaces" */
            int& vertex = verticesOnAllFaces[faceIndex][vertexId];
            /* We'll proceed only if all the faces have not been numbered yet and 
            there are adjacent vertices to "vertex" */
            if(newFacesFound < numberNewFaces && adjacencyList[vertex].size() > 0) 
            {
                /* We'll look for the extrema of the face inside the adjacent vertices of vertex */
                for(int& vertexToCheck1 : adjacencyList[vertex])
                {
                    for(int& vertexToCheck2 : adjacencyList[vertex])
                    {
                        /* We'll proceed only if all of the three vertices are distinct */
                        if(vertex != vertexToCheck1 && vertex != vertexToCheck2 && vertexToCheck1 != vertexToCheck2)
                        {
                            /*Let's rename the "edges" of the face for code readability */
                            int& e1 = newMatrEdgeVertices(vertex, vertexToCheck1);
                            int& e2 = newMatrEdgeVertices(vertexToCheck1, vertexToCheck2);
                            int& e3 = newMatrEdgeVertices(vertexToCheck2, vertex);

                            /* We'll proceed only if the edges that connect the vertices exist */
                            if(e1 >= 0 && e2 >= 0 && e3 >= 0) 
                            {
                                array<int, 3> sortedVertFace = {vertex, vertexToCheck1, vertexToCheck2};
                                /* We'll sort the face (based on the indexes of its vertices) 
                                in order to avoid counting multiple times the same faces 
                                with different vertex ordering */
                                sort(sortedVertFace.begin(), sortedVertFace.end()); 
    
                                /* We'll proceed only if the sorted triangle is not already in the vector */
                                if(find(vecVertFaces.begin(), vecVertFaces.end(), sortedVertFace) == vecVertFaces.end())
                                {
                                    /* We add the face the the structure storing unique faces (in order 
                                    to find its copies after this iteration) */
                                    vecVertFaces.push_back(sortedVertFace);
                                    
                                    // Just to have the edges with the smaller indexes on the top 
                                    // (we can also erase this next two lines and add to ListEdgeFaces e1, e2 and e3)
                                    array<int, 3> edgesInFace = {e1, e2, e3}; 
                                    sort(edgesInFace.begin(), edgesInFace.end()); 

                                    // array<int, 3> verticesInFace = {vertex, vertexToCheck1, vertexToCheck2};

                                    /* We add the vertices and edges to the old polyhedron data structures, 
                                    modifying them */
                                    newListVertFaces(0, newFacesFound) = vertex;
                                    newListVertFaces(1, newFacesFound) = vertexToCheck1;
                                    newListVertFaces(2, newFacesFound) = vertexToCheck2;

                                    newListEdgeFaces(0, newFacesFound) = edgesInFace[0];
                                    newListEdgeFaces(1, newFacesFound) = edgesInFace[1];
                                    newListEdgeFaces(2, newFacesFound) = edgesInFace[2];
                                    
                                    /* Passing to the next face only if we saved a face during this iteration */
                                    newFacesFound++;
                                }  
                            } 
                        }                    
                    }
                }   
            }
        }
    }

    // cout << "newCoordVertices: \n" << newCoordVertices << endl;
    // cout << "newExtremaEdges: \n" << newExtremaEdges << endl;
    // cout << "MatrEdgeVertices: \n" << newMatrEdgeVertices << endl;
    // cout << "newListVertFaces: \n" << newListVertFaces << endl;
    // cout << "newListEdgeFaces: \n" << newListEdgeFaces << endl;

    Eigen::MatrixXi& newListAdjacentFaces = tessellatedPolyhedron.ListAdjacentFaces;
    newListAdjacentFaces.resize(p, numberNewFaces);
    tessellatedPolyhedron.FindAdjacentFaces();

    /* La lista delle facce adiacenti servirà? */

    return tessellatedPolyhedron;
}

Eigen::VectorXi SortVertices(Eigen::Vector3d& Listvertices)
{
    Eigen::VectorXi SortedVertices;
    double max_height = Listvertices.row(2).maxCoeff();
    double avg_height = Listvertices.row(2).sum()/(Listvertices.size());
    if (max_height > avg_height)
        for (int i = 0; i < Listvertices.row(0).size(); i++)
        {
            for (int j = 0; j < Listvertices.row(0).size() - i - 1; j++)
            {
                if (Listvertices(2, j) > Listvertices(2, j + 1))
                break;
            }
        }

    return SortedVertices;
}

void TypeIITessellation(GEOPolyhedron& polyhedron, int& numberDivisions)
{
    vector<int> Vertices;

    
}

double distanceSquaredBetween(GEOPolyhedron& polyhedron, int& idPoint1, int& idPoint2)
{

    Eigen::MatrixXd& CoordVertices = polyhedron.CoordVertices;

    double& point1XCoord = CoordVertices(0, idPoint1);
    double& point1YCoord = CoordVertices(1, idPoint1);
    double& point1ZCoord = CoordVertices(2, idPoint1);

    double& point2XCoord = CoordVertices(0, idPoint2);
    double& point2YCoord = CoordVertices(1, idPoint2);
    double& point2ZCoord = CoordVertices(2, idPoint2);

    double differenceXCoord = point1XCoord - point2XCoord;
    double differenceYCoord = point1YCoord - point2YCoord;
    double differenceZCoord = point1ZCoord - point2ZCoord;

    double distanceSquared = differenceXCoord * differenceXCoord +
                      differenceYCoord * differenceYCoord + 
                      differenceZCoord * differenceZCoord;

    return distanceSquared;
}

int triangularNumber(int& b) // Funzione inutile al momento, ma magari in futuro potrebbe servire
{
    return ((b + 1) * (b + 2) / 2);
}
