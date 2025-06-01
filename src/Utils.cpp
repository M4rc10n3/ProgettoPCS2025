/* File containing the body of the functions we'll use */

# include "Utils.hpp"
# include "Polyhedra.hpp"
# include "BuildPolyhedra.hpp"
# include <vector>

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

Eigen::Vector3d OntoTheUnitSphere(Eigen::Vector3d& vertex)
{
    // Ricordo di aver letto che la norma "norm" della libreria Eigen non è la norma spettrale di una matrice,
    // è la verma norma di un vettore?
    double norm = vertex.norm();
    if (norm > 1e-16) // Avoids ZeroDivision errors
    {
        vertex /= norm;
    } else {
        vertex.setZero();
    }
    return vertex;

    // TODO: Pare che all'interno di Eigen esista la funzione che normalizza proprio il vettore, 
    // credo sia meglio usarla nel caso in cui funzioni:
    // return vertex.normalized();
}



vector<int> WhichIsTheMinimumPathBetween(int& id_vertex_1, int& id_vertex_2)
{
    // Le due prossime righe servono a rimuovere solo i warning dalla compilazione
    id_vertex_1 = 0;
    id_vertex_2 = 0;

    vector<int> Path;
    return Path;
}

void Dualise(GEOPolyhedron& polyhedron, const int& Schlafli_p, const int& Schlafli_q)
{
    Eigen::MatrixXd CoordVertices(3, polyhedron.NumFaces);
    BuildPolyhedra constructor(Schlafli_q, Schlafli_p); // p, q are switched because it's the dual polyhedron
    // Eigen::MatrixXd CoordVertices;
    // BuildPolyhedra constructor(Schlafli_q, Schlafli_p); // p, q are switched because it's the dual polyhedron

    cout << "\n DualPolyhedron \n" << endl;

    for (int i = 0; i < polyhedron.NumFaces; i++)
    {
        // Eigen::Vector3i dual_vertex  = polyhedron.ListVertFaces.col(i);
        // Eigen::Vector3d barycenter_coordinates = OntoTheUnitSphere(FindBarycenter(polyhedron, dual_vertex));
        //CoordVertices.col(i) = barycenter_coordinates; 
    }

    constructor.PointsPolyhedra(CoordVertices);
    constructor.CreateCells();
    constructor.ExportPolyhedra();
}

void TypeITessellation(GEOPolyhedron& polyhedron, int& numberDivisions)
{
    /* We'll need the number of triangles for some resizing, so let's calculate it using 
    the formulas given in "PCS_Project.pdf" by Miss. Teora */
    
    const int numberTrianglesForFace = numberDivisions * numberDivisions; // T = b^2
    int numberNewVertices = 0; // V = 2 * T + 2
    int numberNewEdges = 0; // E = 6 * T
    int numberNewFaces = 0; // F = 4 * T

    int& q = polyhedron.q;
    /* The formulas for the new values of the polyhedron depend on the value of the Schläfli number q,
    so let's divide the different cases: */
    switch (q)
    {
        case 3:
            numberNewVertices = 2 * numberTrianglesForFace + 2; // V = 2 * T + 2
            numberNewEdges = 6 * numberTrianglesForFace; // E = 6 * T
            numberNewFaces = 4 * numberTrianglesForFace; // F = 4 * T

            cout << "numberNewVertices: " << numberNewVertices << endl;
            cout << "numberNewEdges: " << numberNewEdges << endl;
            cout << "numberNewFaces: " << numberNewFaces << endl;

            break;
        case 4:
            numberNewVertices = 4 * numberTrianglesForFace + 2; // V = 4 * T + 2
            numberNewEdges = 12 * numberTrianglesForFace; // E = 12 * T
            numberNewFaces = 8 * numberTrianglesForFace; // F = 8 * T

            cout << "numberNewVertices: " << numberNewVertices << endl;
            cout << "numberNewEdges: " << numberNewEdges << endl;
            cout << "numberNewFaces: " << numberNewFaces << endl;

            break;
        case 5:
            numberNewVertices = 10 * numberTrianglesForFace + 2; // V = 10 * T + 2
            numberNewEdges = 30 * numberTrianglesForFace; // E = 30 * T
            numberNewFaces = 20 * numberTrianglesForFace; // F = 20 * T

            cout << "numberNewVertices: " << numberNewVertices << endl;
            cout << "numberNewEdges: " << numberNewEdges << endl;
            cout << "numberNewFaces: " << numberNewFaces << endl;

            break;
    }

    /* We'll use the old polyhedron data structures to save the new vertices, edges and faces, so
    let's rename them for code readability and resize them (conservatively) using the values 
    calculated above. */
    Eigen::MatrixXd& CoordVertices = polyhedron.CoordVertices;
    CoordVertices.conservativeResize(3, numberNewVertices);
    /* CoordVertices will be divided like so:
        - the first columns (they will be a quantity equal to oldNumVertices) will be for the 
            coordinates of the vertices of the old polyhedron;
        - the next columns will be for the coordinates of the vertices on each edge: there will be 
            (numberDivisions - 1) columns for each edge;
        - the last columns of the matrix will be for the coordinates of the inner vertices of each 
            face which we'll no longer access, so we don't care of their order as a function of 
            other values */

    Eigen::MatrixXi& ExtremaEdges = polyhedron.ExtremaEdges;
    ExtremaEdges.conservativeResize(2, numberNewEdges);

    Eigen::MatrixXi& MatrEdgeVertices = polyhedron.MatrEdgeVertices;
    MatrEdgeVertices.conservativeResize(numberNewVertices, numberNewVertices);

    Eigen::MatrixXi& ListVertFaces = polyhedron.ListVertFaces;
    const int& p = ListVertFaces.col(0).size();
    ListVertFaces.conservativeResize(p, numberNewFaces);

    Eigen::MatrixXi& ListEdgeFaces = polyhedron.ListEdgeFaces;
    ListEdgeFaces.conservativeResize(p, numberNewFaces);

    Eigen::MatrixXi& ListAdjacentFaces = polyhedron.ListAdjacentFaces;
    ListAdjacentFaces.conservativeResize(p, numberNewFaces);

    const int& oldNumFaces = polyhedron.NumFaces;
    const int& oldNumEdges = polyhedron.NumEdges;
    const int& oldNumVertices = polyhedron.NumVertices;
    double& oldLengthEdge = polyhedron.lengthEdge;

    /* We'll need the length of the edge of the new polyhedron in order to find the inner vertices 
    of each face and most importantly all of its edges, so let's compute it: */
    double newLengthEdge = oldLengthEdge / numberDivisions; 
    cout << "newLengthEdge: " << newLengthEdge << endl;

    /* Let's initialize a vector that will save all of the edges that we have already divided.
    It will start with just "-1" as its elements. Then, we'll also need to keep track of its 
    index in order to increase it and to access the right element of the vector. */
    vector<int> edgesDone(oldNumEdges, -1);
    int edgesDoneIndex = 0;

    /* IN order not to get it out of scope, we need to initialize here the ariable we'll 
    use to keep track of how many inner vertices we have found and saved. This variable 
    will be uesed to access the right column of the matrix "CoordVertices" */
    int innerVerticesSaved = 0;

    /* We need to tesselate each face of the polyhedron given as input, so we need a "for" 
    cycle on the old polyhedron faces: */
    for(int faceIndex = 0; faceIndex < oldNumFaces; faceIndex++)
    {
        Eigen::VectorXd newXCoordVerticesOnEdge = VectorXd::Zero(numberDivisions + 1);
        Eigen::VectorXd newYCoordVerticesOnEdge = VectorXd::Zero(numberDivisions + 1);
        Eigen::VectorXd newZCoordVerticesOnEdge = VectorXd::Zero(numberDivisions + 1);

        /* In order to tesselate, we need to divide each edge into "numberDivisions" parts,
         so we need a "for" cycle on the edges of each face: */
        for(int edge = 0; edge < p; edge++)
        {
            /* Let's find the index we used in our data structures for the edge of the face: */
            int& edgeIndex = ListEdgeFaces(edge, faceIndex);
            
            /* Let's skip the edges we've already divided */
            if(find(edgesDone.begin(), edgesDone.end(), edgeIndex) != edgesDone.end()){
                continue;
            }      
            else
            {
                /* Using the edgeIndex we found above we can access the right vertices 
                indexes inside our data structures and rename them for code readability: */
                int& firstVertexWeAreDividing = ExtremaEdges(0, edgeIndex);
                int& secondVertexWeAreDividing = ExtremaEdges(1, edgeIndex);

                /* Let's rename the coordinates of the two vertices for code readability: */
                double& firstVertexXCoord = CoordVertices(0, firstVertexWeAreDividing);
                double& firstVertexYCoord = CoordVertices(1, firstVertexWeAreDividing);
                double& firstVertexZCoord = CoordVertices(2, firstVertexWeAreDividing);

                double& secondVertexXCoord = CoordVertices(0, secondVertexWeAreDividing);
                double& secondVertexYCoord = CoordVertices(1, secondVertexWeAreDividing);
                double& secondVertexZCoord = CoordVertices(2, secondVertexWeAreDividing);

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

                    CoordVertices(0, newVertexId) = newXCoordVerticesOnEdge(division + 1);
                    CoordVertices(1, newVertexId) = newYCoordVerticesOnEdge(division + 1);
                    CoordVertices(2, newVertexId) = newZCoordVerticesOnEdge(division + 1);
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
        For b = 4, we'll firstly look for those that have a distance equal to 3 times that of the edge, 
        then we'll search for those that have a distance equal to 2 times that of the edge.
        Once we'll have found the two vertices, we'll use the same algorithm as above to find the inner 
        vertices subdividing the segment that has the two vertices as extrema */

        /* We'll always start our search from the vertices on the first edge of the face and we'll look for 
        the vertices on the other edges of the same face */
        int& edgeWeStartFromIndex = ListEdgeFaces(0, faceIndex);
        for(int time = 0; time < numberDivisions - 2; time++)
        /* "time" represents the number of inner segmentsthat each face has */
        {
            /* We'll look for the right vertex on both the opposing vertices: */
            for(int opposingEdgeIndex = 1; opposingEdgeIndex < p; opposingEdgeIndex++){
                /* Let's find the right index of the edge accessing our data structures:  */
                int& opposingEdge = ListEdgeFaces(opposingEdgeIndex, faceIndex);

                /* Now we can look for the right vertex on the edge: */
                for(int idVertexOnEdge = 0; idVertexOnEdge < (numberDivisions - 1); idVertexOnEdge++)
                {
                    /* In order to access the proper vertex where we'll start from, we should ignore:
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
                    
                    /* We'll go on with the algorithm only if the distance between to the two vertices 
                    is equal to that we're looking for: */
                    if(abs(distanceSquaredBetween(polyhedron, idVertexWeStartFrom, idOpposingVertex) - 
                        newLengthEdge * (numberDivisions - 1 - time)) < 1e-1)
                        /* The tolerance was found thanks to trial and error. It is big because 
                        our coordinates have 2 decimal digits */
                        {
                            /* Now that we've found the 2 vertices, we'll repeat the algorithm 
                            as above in order to find the coordinates of the inner vertices */
                            int& firstVertexWeAreDividing = idVertexWeStartFrom;
                            int& secondVertexWeAreDividing = idOpposingVertex;

                            double& firstVertexXCoord = CoordVertices(0, firstVertexWeAreDividing);
                            double& firstVertexYCoord = CoordVertices(1, firstVertexWeAreDividing);
                            double& firstVertexZCoord = CoordVertices(2, firstVertexWeAreDividing);

                            double& secondVertexXCoord = CoordVertices(0, secondVertexWeAreDividing);
                            double& secondVertexYCoord = CoordVertices(1, secondVertexWeAreDividing);
                            double& secondVertexZCoord = CoordVertices(2, secondVertexWeAreDividing);

                            newXCoordVerticesOnEdge = VectorXd::LinSpaced(numberDivisions - time, 
                                                                        firstVertexXCoord, 
                                                                        secondVertexXCoord);
                            newYCoordVerticesOnEdge = VectorXd::LinSpaced(numberDivisions - time, 
                                                                        firstVertexYCoord,
                                                                        secondVertexYCoord);
                            newZCoordVerticesOnEdge = VectorXd::LinSpaced(numberDivisions - time, 
                                                                        firstVertexZCoord, 
                                                                        secondVertexZCoord);

                            /* We also need to save the new vertices, with a different algorithm compared 
                            to the one above: */
                            for(int vertexSaved = 0; vertexSaved < numberDivisions - 2 - time; vertexSaved++)
                            {
                                /* In order to access the proper vertex where we'll end at, we should ignore:
                                    - the vertices of the old polyhedron;
                                    - all of the vertices we found above on the edges of the old polyhedron; 
                                then we should access the vertex with id "innerVerticesSaved", variable 
                                initialised outside the "for" cycle on the faces of the old polyhedron. 
                                Each of these members is represented in a line in the next sum for 
                                code readability: */
                                int newVertexId = oldNumVertices + 
                                                  (numberDivisions - 1) * oldNumEdges + 
                                                  innerVerticesSaved;
                                
                                CoordVertices(0, newVertexId) = newXCoordVerticesOnEdge(vertexSaved + 1);
                                CoordVertices(1, newVertexId) = newYCoordVerticesOnEdge(vertexSaved + 1);
                                CoordVertices(2, newVertexId) = newZCoordVerticesOnEdge(vertexSaved + 1);

                                /* Now that we've found an inner vertex, we can increase the variable 
                                that counts them: */
                                innerVerticesSaved++;
                            }
                        break;
                    }
                }
            }
        }
    }

    /* Now we'll find the polyhedron new edges using the same algorithm used for the old polyhedra 
    based on the length of the edge. This variable be the only thing that will change*/
        
    /* We need to keep track of how many edges we've found, in order not to find repeating edges: */
    int edgeIndexFound = 0;

    /* We need to find the edges that start from each vertex (except for the last one, 
    because that would be a certain useless iteration: we'll have already found all of the edges 
    that have the last vertex as an extrema) */
    
    // TODO: Inserire un if che faccia salvare il lato solo se i vertici 
    // appartengono alla stessa faccia del vecchio poliedro
    
    for(int firstVertexIndex = 0; firstVertexIndex < numberNewVertices - 1; firstVertexIndex++)
    {
        /* Proceed only if all the edges have not been numbered yet */
        if(edgeIndexFound <= numberNewEdges)
        {
            /* We'll check every other vertex of the polyhedron (for which we don't have already 
            found all edges) in order to find the ones with the exact distance from the vertex 
            with index "firsteVertexIndex" */
            for(int secondVertexIndex = firstVertexIndex + 1; secondVertexIndex < numberNewVertices; secondVertexIndex++)
            {
                /* We'll use a function we have implemented in "Utils.cpp" in order to find the 
                distance squared between the two vertices: */
                double distanceSquared = distanceSquaredBetween(polyhedron, firstVertexIndex, secondVertexIndex);

                cout << "firstVertexIndex: " << firstVertexIndex << endl;
                cout << "secondVertexIndex: " << secondVertexIndex << endl;
                cout << "abs(distanceSquared - newLengthEdge * newLengthEdge): " << abs(distanceSquared - newLengthEdge * newLengthEdge) << endl;
                /* When the two vertices have the correct distance squared between them we save them as an 
                edge of the polyhedron (we decided to set this small tolerance because the data 
                we used to create the polygons has two decimal digits) */

                // Con 157e-5 trova 41 lati, ma con 158e-5 esplode
                if(abs(distanceSquared - newLengthEdge * newLengthEdge) <= 157e-5)
                {
                    cout << "edgeIndexFound: " << edgeIndexFound << endl;
                    //Qua sotto c'è un problema per edgeIndexFound = 48, che fa accedere a ExtremaEdges(54)

                    cout << "oldNumEdges + edgeIndexFound: " << oldNumEdges + edgeIndexFound << endl;
                    ExtremaEdges(0, oldNumEdges + edgeIndexFound) = firstVertexIndex;
                    ExtremaEdges(1, oldNumEdges + edgeIndexFound) = secondVertexIndex;
                    cout << "Salvo gli estremi" << endl;
                    /* We also save the index of the edge inside the matrix "MatrEdgeVertices": 
                    we'll use it in order to find the adjacent vertices for each vertex and 
                    the faces of the polyhedron */
                    
                    // Probabilmente MatrEdgeVertices avrà tanti 0 e non -1 come sarebbe meglio, 
                    // bisogna fare in modo che abbi a -1 quasi dappertutto
                    
                    MatrEdgeVertices(firstVertexIndex, secondVertexIndex) = oldNumEdges + edgeIndexFound;
                    MatrEdgeVertices(secondVertexIndex, firstVertexIndex) = oldNumEdges + edgeIndexFound;

                    /* Now that we've found an edge we can go on to the next edge: */
                    edgeIndexFound++;
                }
            }
        }
    }
    cout << "158e-5: " << 158e-5 << endl;
    cout << "edgeIndexFound: " << edgeIndexFound << endl;
    cout << "ExtremaEdges:\n " << ExtremaEdges << endl;
    cout << "MatrEdgeVertices:\n " << MatrEdgeVertices << endl;

}



// void TypeIITessellation(GEOPolyhedron& polyhedron)
// {
    
// }

double distanceSquaredBetween(GEOPolyhedron& polyhedron, int& idPoint1, int& idPoint2){

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

// Funzione inutile al momento, ma magari in futuro potrebbe servire
int triangularNumber(int& b){
    return ((b + 1) * (b + 2) / 2);
}
