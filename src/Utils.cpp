/* File containing the body of the functions we'll use */
# include <vector>
# include <queue>
# include "Utils.hpp"
# include "Polyhedra.hpp"

using namespace PolyhedraLibrary;
using namespace std;
using namespace Eigen;

Eigen::Vector3d findBarycenter(GEOPolyhedron& polyhedron, Eigen::Vector3i& VertFace)
{
    /* Let's initialise a vector where the coordinates of the barycenter will be stored and 
    let's rename its components for code readability */
    Eigen::Vector3d barycenterCoordinates = Eigen::Vector3d::Zero();
    double& CoordX = barycenterCoordinates(0);
    double& CoordY = barycenterCoordinates(1);
    double& CoordZ = barycenterCoordinates(2);

    /* Let's find the coordinates of the barycenter by computing the mean of the coordinates 
    of the 3 vertices of the triangular face of the polyhedron */
    for(int i = 0; i < 3; i++)
    {
        CoordX += polyhedron.CoordVertices(0, VertFace(i));
        CoordY += polyhedron.CoordVertices(1, VertFace(i));
        CoordZ += polyhedron.CoordVertices(2, VertFace(i));
    }
    barycenterCoordinates = {CoordX/3.0, CoordY/3.0, CoordZ/3.0};

    /* Now that everything's done, we can return "barycenterCoordinates" */
    return barycenterCoordinates;
}

vector<int> BFS(const vector<vector<int>>& adjList, const int& v1, const int& v2, const int& numVert, const double& lengthEdge, double& lengthPath)
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

                lengthPath = (minPath.size() - 1) * lengthEdge; // calculate the minimum path's length
                
                return minPath; // return the list of vertices in the minimum path            
            }
        }
    }
    return {}; // if no path was found, return an empty vector  
}

vector<int> Dijkstra(const vector<vector<int>>& adjList, const int& v1, const int& v2, const int& numVert, Eigen::MatrixXd& matrWeights, double& lengthPath)
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

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq; // std::greater<int> makes the max priority queue act as a min priority queue;
                                                                                                 // std::pair<T1, T2> is a class template that provides a way to store two heterogeneous objects as a single unit,
                                                                                                 // in this case it stores the distance of each vertex from the source v1 (as the first element,
                                                                                                 // since it represents the priority in the queue) and the vertex id as the second element
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
    
    // Calculate the minimum path's length
    for(unsigned int i = 1; i < minPath.size(); i++)
    {
        lengthPath += matrWeights(minPath[i-1], minPath[i]);
    }                        

    return minPath; // return the list of vertices in the minimum path                  
}

void MinimumPath(const vector<int>& minPath, 
                 const Eigen::MatrixXi& MatrEdgeVertices, 
                 const int& numVert, 
                 const int& numEdge, 
                 const double& lengthPath, 
                 vector<double>& verticesShortPath, 
                 vector<double>& edgesShortPath)
{
    verticesShortPath.resize(numVert, 0.0);
    edgesShortPath.resize(numEdge, 0.0);

    for(auto vertexId : minPath){
        verticesShortPath[vertexId] = 1.0;
    }

    for(unsigned int vertexId = 0; vertexId < minPath.size() - 1; vertexId++){
        const int& vertex1 = minPath[vertexId];
        const int& vertex2 = minPath[vertexId + 1];
        const int& edgeId = MatrEdgeVertices(vertex1, vertex2);
        edgesShortPath[edgeId] = 1.0;
    }
    
    // Output the minimum path (through the ids of the vertices that compose it) and its length
    cout << "The minimum path is: ";
    for(int i : minPath)
        cout << i << " ";
    cout << endl;
    cout << "The minimum path is composed of " << minPath.size() - 1 << " edges" << endl;
    cout << "The minimum path length is " << lengthPath << endl;
}

void OntoTheUnitSphere(GEOPolyhedron& polyhedron)
{
    /* Let's rename the data structures we'll use in this function for code readability */
    Eigen::MatrixXd& CoordVertices = polyhedron.CoordVertices;
    int& NumVertices = polyhedron.NumVertices;

    /* We need to normalize the vector of the coordinates of each vertex, so we need a 
    "for" cycle on the indices of the vertices */
    for(int vertexIndex = 0; vertexIndex < NumVertices; vertexIndex++)
    {
        /* Let's initialise a vector where the coordinates of the vector will be saved */
        Eigen::Vector3d vertexCoord;
        vertexCoord << CoordVertices(0, vertexIndex),
                       CoordVertices(1, vertexIndex),
                       CoordVertices(2, vertexIndex);

        /* Let's normalise and update the vector of the coordinates using the ".normalized()" 
        method already implemented in the Eigen library */
        vertexCoord = vertexCoord.normalized();

        /* Let's save the new coordinates of the vertex inside the "Coordvertices" matrix of the polyhedron,
        updating it */
        for(int coord = 0; coord < 3; coord++)
        {
            CoordVertices(coord, vertexIndex) = vertexCoord(coord);
        }
    }
}

GEOPolyhedron Dualise(GEOPolyhedron& polyhedron)
{
    /* Let's initialise the object of type "GEOPolyhedron" that will be returned by the function */
    GEOPolyhedron dualPolyhedron;

    // polyhedron.FindFacesWithVertex();
    // Questo sarà il vettore di vettori che esporteremo come facce del poliedro
    //vector<vector<unsigned int>>& oldListFacesWithVertex = polyhedron.ListFacesWithVertex;

    /* Let's rename the data structures we'll use in this function for code readability */
    Eigen::MatrixXi& oldListAdjacentFaces = polyhedron.ListAdjacentFaces;
    Eigen::MatrixXi& oldListVertFaces = polyhedron.ListVertFaces;
    int& oldNumVertices = polyhedron.NumVertices;
    int& NumEdges = polyhedron.NumEdges;
    int& oldNumFaces = polyhedron.NumFaces;
    
    /* Let's initialise the data structures of the dual polyhedron: the dual polyhedron will 
    have the number of vertices and faces swapped in comparison to the original polyhedron */
    dualPolyhedron.NumVertices = oldNumFaces;
    dualPolyhedron.NumEdges = NumEdges;
    dualPolyhedron.NumFaces = oldNumVertices;

    Eigen::MatrixXd& newCoordVertices = dualPolyhedron.CoordVertices;
    newCoordVertices.resize(3, dualPolyhedron.NumVertices);

    Eigen::MatrixXi& newExtremaEdges = dualPolyhedron.ExtremaEdges;
    newExtremaEdges.resize(2, NumEdges);

    Eigen::MatrixXi& newListVertFaces = dualPolyhedron.ListVertFaces;
    newListVertFaces.resize(3, dualPolyhedron.NumFaces);

    /* Let's initialise a structure we'll use in order to find the edges and not repeat them */
    vector<array<int, 2>> edgesFound;
    /* The structure will need just enough memory space to save all of edges (that are represented
    by their extrema) */
    edgesFound.reserve(NumEdges);



    // Questa variabile potrebbe essere rimpiazzata con la size() di edgesFound, che dite? 
    // Risparmiamo memoria o potenza di calcolo?
    int edgeFoundIndex = 0;



    /* For each face we'll calculate the coordinates of its barycenter and the edges that 
    have it as one of their extrema */
    for(int faceIndex = 0; faceIndex < oldNumFaces; faceIndex++)
    {
        /* Let's initialise a vector that will store the indexes of the vertices of the face */
        Eigen::Vector3i verticesOfFace = Eigen::Vector3i::Zero();
        verticesOfFace << oldListVertFaces(0, faceIndex),
                          oldListVertFaces(1, faceIndex),
                          oldListVertFaces(2, faceIndex);

        /* Let's use the function "findBarycenter" we implemented in order to compute 
        the barycenter coordinates */
        Eigen::Vector3d barycenterCoordinates = findBarycenter(polyhedron, verticesOfFace);
        /* Let's save the coordinates of the barycenter of the face with index "faceIndex" as the 
        coordinates of the vertex with index "faceIndex" of "dualPolyhedron" */
        newCoordVertices.col(faceIndex) = barycenterCoordinates;
        
        /* In order to minimize the computational strain, let's find the edges that have the barycenter
        with index "faceIndex" as one of their extrema. By using the structure "oldListAdjacentFaces" we 
        can know which faces are adjacent, therefore connect their barycenters and create an edge. 
        We just need to make sure that we didn't save the edge yet in order to avoid repeating edges. */
        for(int adjacentFace = 0; adjacentFace < 3; adjacentFace++)
        {
            /* Let's access the right "adjacentFaceIndex" using the structure "oldListAdjacentFaces"*/
            int& adjacentFaceIndex = oldListAdjacentFaces(adjacentFace, faceIndex);

            /* An edge is formed by the two adjacent faces */
            array<int, 2> sortedEdge = {faceIndex, adjacentFaceIndex};
            /* We sort it in order to save it inside the structure "edgesFound" just once */
            sort(sortedEdge.begin(), sortedEdge.end());

            if(find(edgesFound.begin(), edgesFound.end(), sortedEdge) == edgesFound.end()){
                /* If the edge isn't in the vector yet, we save it inside of it and in all of our 
                data structures */
                edgesFound.push_back(sortedEdge);

                newExtremaEdges(0, edgeFoundIndex) = sortedEdge[0];
                newExtremaEdges(1, edgeFoundIndex) = sortedEdge[1];
                
                /* Let's update the counter of how many edges we've found in order to access the right 
                column of the matrix "newExtremaEdges" */
                edgeFoundIndex++;
            }
        }
    }

    /* Now that everything's done, we can return "dualPolyhedron" */
    return dualPolyhedron;
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
    
    /* We need to copy these two variables into the new polyhedron in order to make other functions 
    (needed to dualise the polyhedron) work */
    tessellatedPolyhedron.q = polyhedron.q;
    tessellatedPolyhedron.p = polyhedron.p;
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

    const int numberVerticesOnFace = ((numberDivisions + 1) * (numberDivisions + 2) / 2);

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
    
    int faceCounter = 0;
    
    /* Let's initialise the vector which stores unique triangles (the faces of the polyhedron) 
    as sorted arrays of 3 vertices. It will store the unique new faces found on the old polyhedron 
    face with index "faceIndex" */
    vector<array<int, 3>> vecVertFaces;
    vecVertFaces.reserve(numberNewFaces / oldNumFaces);
    
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
                        double distance = abs(DistanceSquaredBetween(tessellatedPolyhedron, idVertexWeStartFrom, idOpposingVertex) - 
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
                    double distance = abs(DistanceSquaredBetween(tessellatedPolyhedron, idVertexWeStartFrom, idOpposingVertex) - 
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
                    double distanceSquared = DistanceSquaredBetween(tessellatedPolyhedron, firstVertexIndex, secondVertexIndex);

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
        
        /* Now we can look for the new faces of the polyhedron v1ing from each of the vertices 
        on the face of the old polyhedron */
        int numAdjacentVertices = 6;
        tessellatedPolyhedron.FindFaces(verticesOnFace, faceCounter, vecVertFaces, numAdjacentVertices);
    }
    Eigen::MatrixXi& newListAdjacentFaces = tessellatedPolyhedron.ListAdjacentFaces;
    newListAdjacentFaces.resize(p, numberNewFaces);
    tessellatedPolyhedron.FindAdjacentFaces();

    return tessellatedPolyhedron;
}

GEOPolyhedron TypeIITessellation(GEOPolyhedron& polyhedron, GEOPolyhedron& tessellatedPolyhedron, int& numberDivisions)
{
    GEOPolyhedron GEOSolid;

    int& NumVertices = GEOSolid.NumVertices;
    int& NumEdges = GEOSolid.NumEdges;
    int& NumFaces = GEOSolid.NumFaces;
    NumVertices = 0;
    NumEdges = 0; 
    NumFaces = 0; 

    int& p = polyhedron.p;

    // NumVertices = numV + numE(2b-1)+numF((3b^2)/2-3b/2+1)
    NumVertices = polyhedron.NumVertices + 
                  (polyhedron.NumEdges * ((2 * numberDivisions)-1)) + 
                  (polyhedron.NumFaces * (((3 * numberDivisions * numberDivisions)/ 2.0) 
                                            - ((3 * numberDivisions)/ 2.0) + 1)); 

    // NumEdges = numE(2b)+numF((9b^2)/2+3b/2) 
    NumEdges = (polyhedron.NumEdges * 2 * numberDivisions) + 
               (polyhedron.NumFaces * (((9 * numberDivisions * numberDivisions)/ 2.0) 
                                         + (3 * numberDivisions)/2.0)); 

    // Numfaces = numF((3b^2)+3b)
    NumFaces = polyhedron.NumFaces * ((3 * numberDivisions * numberDivisions) + 3 * numberDivisions); 

    cout << "NumVertices: " << NumVertices << endl;
    cout << "NumEdges: " << NumFaces << endl;
    cout << "NumEdges: " << NumFaces << endl;


    unsigned int FacesperFace = tessellatedPolyhedron.NumFaces/polyhedron.NumFaces; // Number of polygons per face derived by Tesselation I

    vector<array<int, 3>> vecVertFaces;
    vecVertFaces.reserve(NumFaces / polyhedron.NumFaces); // this array is needed to store the new faces 
                                                          // on each face of the old polyhedron

    GEOSolid.CoordVertices = MatrixXd::Zero(3, NumVertices);
    GEOSolid.ExtremaEdges = MatrixXi::Zero(2, NumEdges);
    GEOSolid.MatrEdgeVertices = MatrixXi::Constant(NumVertices, NumVertices, -1);
    // GEOSolid.MatrEdgeVertices.setConstant(-1);
    GEOSolid.ListEdgeFaces = MatrixXi::Zero(polyhedron.p, NumFaces);
    GEOSolid.ListVertFaces = MatrixXi::Zero(polyhedron.p, NumFaces);
    GEOSolid.lengthEdge = polyhedron.lengthEdge/(sqrt(3) * numberDivisions); // maximum distance between two point (diagonaly)

    double second_length_edge = polyhedron.lengthEdge/(2 * numberDivisions); // length between two edge vertices


    VectorXi edgetracker = VectorXi::Zero(polyhedron.NumEdges); // prevents generating copies of edge points
    // 0 if an edge in not been yet considered 

    vector<bool> vertexbool(tessellatedPolyhedron.NumVertices); // checked vertices of the tesselated polyhedron
    // useful for recognizing all the vertices that are inside of the polyhedron

    MatrixXi MatEdgeVertices = MatrixXi::Zero(polyhedron.NumEdges, (2 * numberDivisions) - 1); // Every row of the matrix
    // is the id of each edge of the initial polyhedron; 

    int vertexcounter = 0; // takes track of the number of vertices examined
    int edgecounter = 0; // takes track of the number of edges examined

    int faceCounter = 0; // takes track of the number of faces found


    // Facendo un'uguaglianza non funziona, vero? Usando un "block" o un "leftCols"?
    for (int i = 0; i < polyhedron.NumVertices; i++)
    {
        GEOSolid.CoordVertices.col(i) = polyhedron.CoordVertices.col(i);
        vertexcounter++;
    }

    for (int i = 0; i < polyhedron.NumFaces; i++)
    {
        // First we divide points between edgepoints and innerpoints
        vector<Vector3d> innerpoints;
        vector<Vector3d> edgepoints;
        vector<int> total_vertices;
        vector<int> IDbarycenters;
        vector<int> IDedgepoints;

        int additional_barycenters = 0; // if b >= 3 some vertices of the face are not on the edges so they must be counted

        if (numberDivisions >= 3)
        {
            // Volendo, possiamo creare una funzione che restituisca il numero triangolare che 
            // rappresenta il numero di "additional_barycenters"; io sicuramente ho usato
            // un'altra volta i numeri triangolari nella tassellazione I
            additional_barycenters = ((numberDivisions - 1) * (numberDivisions - 2))/2;
        }

        for (int z = 0; z < p; z++)
        {
            edgepoints.push_back(polyhedron.CoordVertices.col(polyhedron.ListVertFaces(z,i))); // stores the initial vertices
            IDedgepoints.push_back(polyhedron.ListVertFaces(z,i));
            total_vertices.push_back(polyhedron.ListVertFaces(z,i));
        }

        for (int k = 0; k < p; k++) // generates the edgepoints of a Face
        {
            int& edge = polyhedron.ListEdgeFaces(k, i);
            if (edgetracker(edge) == 0)
            {
                int& IDVertex_1 = polyhedron.ExtremaEdges(0, edge);
                int& IDVertex_2 = polyhedron.ExtremaEdges(1, edge);

                double& x_1 = polyhedron.CoordVertices(0, IDVertex_1);
                double& y_1 = polyhedron.CoordVertices(1, IDVertex_1);
                double& z_1 = polyhedron.CoordVertices(2, IDVertex_1);

                double& x_2 = polyhedron.CoordVertices(0, IDVertex_2);
                double& y_2 = polyhedron.CoordVertices(1, IDVertex_2);
                double& z_2 = polyhedron.CoordVertices(2, IDVertex_2);

                Vector3d angularcoefficient = Vector3d::Zero(); // angular coefficient of the line between the two vertices
                angularcoefficient(0) = x_1 - x_2;
                angularcoefficient(1) = y_1 - y_2;
                angularcoefficient(2) = z_1 - z_2;

                for (int w = 0; w < (2 * numberDivisions - 1); w++)
                {
                    double t = second_length_edge / sqrt(angularcoefficient.squaredNorm()); // t = d / sqrt(v1^2 + v2^2 + v3^2)
                    double x = x_2 + t * angularcoefficient(0);
                    double y = y_2 + t * angularcoefficient(1);
                    double z = z_2 + t * angularcoefficient(2);

                    GEOSolid.CoordVertices.col(vertexcounter) = Vector3d(x, y, z);

                    edgepoints.push_back(Vector3d(x, y, z));
                    MatEdgeVertices(edge, w) = vertexcounter;
                    
                    if (w == 0)
                    {
                        GEOSolid.ExtremaEdges(0, edgecounter) = IDVertex_2;
                        GEOSolid.ExtremaEdges(1, edgecounter) = vertexcounter;

                    } else {

                        GEOSolid.ExtremaEdges(0, edgecounter) = vertexcounter - 1;
                        GEOSolid.ExtremaEdges(1, edgecounter) = vertexcounter;

                    }
                    IDedgepoints.push_back(vertexcounter);
                    total_vertices.push_back(vertexcounter);

                    x_2 = x;
                    y_2 = y;
                    z_2 = z;

                    vertexcounter++;
                    edgecounter++;
                }
                // Non ho capito questo prossimo commento: il "for" non si ferma dopo che ha generato 
                // l'ultimo punto sul lato del vecchio poliedro e quindi ci serve solo aggiungere 
                // il lato sul nuovo poliedro tra quest'ultimo vertice e IDVertex_1?

                // the for cycle stops before the last vertex so  manually add the last edge
                GEOSolid.ExtremaEdges(0, edgecounter) = vertexcounter - 1; 
                GEOSolid.ExtremaEdges(1, edgecounter) = IDVertex_1;

                edgecounter++;
                edgetracker(edge) = 1;

            } else { // edge already considered before

                for (int w = 0; w < (2 * numberDivisions - 1); w++)
                {
                    // Perché ci serve risalvare dati già prodotti? Per fare meno casino con indici dopo?
                    edgepoints.push_back(GEOSolid.CoordVertices.col(MatEdgeVertices(edge,w)));
                    IDedgepoints.push_back(MatEdgeVertices(edge, w));
                    total_vertices.push_back(MatEdgeVertices(edge, w));
                }
            }
        }

        for (unsigned int j = 0; j < FacesperFace; j++) // generates the innerpoints of a Face
        {
            Vector3i VertFace = Vector3i::Zero();

            VertFace(0) = tessellatedPolyhedron.ListVertFaces(0, j + (i * FacesperFace));
            VertFace(1) = tessellatedPolyhedron.ListVertFaces(1, j + (i * FacesperFace));
            VertFace(2) = tessellatedPolyhedron.ListVertFaces(2, j + (i * FacesperFace));

            // Possiamo sostituire "VertFace.size()" con 3?
            for (int y = 0; y < VertFace.size(); y++) // checks if a vertex of Vertface is on the edge of the face or inside
            {
                if (vertexbool[VertFace(y)] == false)
                {
                    bool flag = false; // becomes true if a point is on the edge;
                    int numedgepoints = IDedgepoints.size();
                    for (int h = 0; h < numedgepoints; h++)
                    {
                        Vector3d norm = tessellatedPolyhedron.CoordVertices.col(VertFace(y)) - GEOSolid.CoordVertices.col(IDedgepoints[h]);
                        if (norm.norm() < 1e-8)
                        {
                            flag = true; // the vertex is on the edge
                            break;
                        }
                    }

                    if (flag == false) // if the vertex is not on the edge is going to be added to the list of barycenters
                    {
                        GEOSolid.CoordVertices.col(vertexcounter) = tessellatedPolyhedron.CoordVertices.col(VertFace(y));
                        innerpoints.push_back(tessellatedPolyhedron.CoordVertices.col(VertFace(y)));

                        additional_barycenters--;
                        IDbarycenters.push_back(vertexcounter);
                        total_vertices.push_back(vertexcounter);
                        vertexcounter++;
                    }
                    vertexbool[VertFace(y)] = true;
                }
            }
            
            Vector3d barycenter = findBarycenter(tessellatedPolyhedron, VertFace);

            GEOSolid.CoordVertices.col(vertexcounter) = barycenter;
            innerpoints.push_back(barycenter);
            IDbarycenters.push_back(vertexcounter);
            total_vertices.push_back(vertexcounter);

            vertexcounter++;
        }

        int numbarycenters = innerpoints.size();
        int numedgepoints = edgepoints.size();
        double distance;

        // Valence of each point on the edge, max = 3, considering a single face and just the barycenters
        VectorXi valenceedgepoints = VectorXi::Zero(numedgepoints); 
        VectorXi valenceinnerpoints = VectorXi::Zero(numbarycenters); 

        for (int j = 0; j < numbarycenters; j++)
        {
            for (int k = 0; k < numedgepoints; k++)
            {
                distance = (edgepoints[k] - innerpoints[j]).norm();
                if (distance < GEOSolid.lengthEdge + 1e-6)
                {
                    GEOSolid.ExtremaEdges(0, edgecounter) = IDbarycenters[j];
                    GEOSolid.ExtremaEdges(1, edgecounter) = IDedgepoints[k];
                    GEOSolid.MatrEdgeVertices(IDbarycenters[j], IDedgepoints[k]) = edgecounter;
                    GEOSolid.MatrEdgeVertices(IDedgepoints[k], IDbarycenters[j]) = edgecounter;

                    valenceinnerpoints(j)++;
                    edgecounter++;
                }
            }

            for (int k = 0; k < numbarycenters; k++) // generates the edges of the polyhedron
            {   
                distance = (innerpoints[k] - innerpoints[j]).norm();

                int flag_edge = GEOSolid.MatrEdgeVertices(IDbarycenters[k], IDbarycenters[j]);

                if (distance < GEOSolid.lengthEdge + 1e-6 && flag_edge == -1 && k != j)
                {
                    GEOSolid.ExtremaEdges(0, edgecounter) = IDbarycenters[j];
                    GEOSolid.ExtremaEdges(1, edgecounter) = IDbarycenters[k];
                    GEOSolid.MatrEdgeVertices(IDbarycenters[k], IDbarycenters[j]) = edgecounter;
                    GEOSolid.MatrEdgeVertices(IDbarycenters[j], IDbarycenters[k]) = edgecounter;

                    valenceinnerpoints(j)++;
                    valenceinnerpoints(k)++;

                    edgecounter++;
                }
            }
            if (valenceinnerpoints(j) < 6)
            {
                cout << "\n The barycenter ID " << IDbarycenters[j] << " didn't find all the 6 near vertices, he made just " <<
                valenceinnerpoints(j) << " edges \n" << endl;

                cout << "The iteration is: " << j << "\n" << endl;
            }
            // innerpoints.erase(innerpoints.begin()); // pop the barycenter just considered to speed up the process
        }

        cout << "CoordVertices: " << endl << GEOSolid.CoordVertices << endl;
        cout << "ExtremaEdges: " << endl << GEOSolid.ExtremaEdges << endl;
        cout << "MatrEdgeVertices: " << endl << GEOSolid.MatrEdgeVertices << endl;
        

        int numAdjacentFaces = 6;
        GEOSolid.FindFaces(total_vertices, faceCounter, vecVertFaces, numAdjacentFaces);
    } 
    return GEOSolid;
}

double DistanceSquaredBetween(GEOPolyhedron& polyhedron, int& idPoint1, int& idPoint2)
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