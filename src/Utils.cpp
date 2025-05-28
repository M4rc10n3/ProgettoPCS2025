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

GEOPolyhedron TypeITessellation(GEOPolyhedron& polyhedron, int& numberDivisions)
{
    /* We'll need the number of triangles for some resizing, so let's calculate it using 
    the formulas given in "PCS_Project.pdf" */
    
    /* Queste sono le formula per il poliedro, bosgna fare in modo di passare alla funzione 
    questi numeri oppure di calcolarli in base a q */
    int numberTrianglesForFace = numberDivisions * numberDivisions; // T = b^2
    // int numberNewVertices = 2 * numberTrianglesForFace + 2; // V = 2 * T + 2
    // Completamente arbitrario, ma corretto per il tetraedro (senza considerare le facce adiacenti):
    int numberNewVertices = 32;
    cout << "numberNewVertices: " << numberNewVertices << endl;
    int numberNewEdges = 6 * numberTrianglesForFace; // E = 6 * T
    int numberNewFaces = 4 * numberTrianglesForFace; // F = 4 * T

    /* Let's rename some data structures for code readability and resize them using 
    the dimensions calculated above*/
    Eigen::MatrixXd& CoordVertices = polyhedron.CoordVertices;
    CoordVertices.conservativeResize(3, numberNewVertices);

//     Eigen::MatrixXi& ExtremaEdges = polyhedron.ExtremaEdges;
//     ExtremaEdges.conservativeResize(2, numberNewEdges);

//     Eigen::MatrixXi& MatrEdgeVertices = polyhedron.MatrEdgeVertices;
//     MatrEdgeVertices.conservativeResize(numberNewVertices, numberNewVertices);

    Eigen::MatrixXi& ListVertFaces = polyhedron.ListVertFaces;
    int& p = ListVertFaces.col.size();
    ListVertFaces.conservativeResize(p, numberNewFaces);

//     Eigen::MatrixXi& ListEdgeFaces = polyhedron.ListEdgeFaces;
//     ListEdgeFaces.conservativeResize(p, numberNewFaces);

//     Eigen::MatrixXi& ListAdjacentFaces = polyhedron.ListAdjacentFaces;
//     ListAdjacentFaces.conservativeResize(p, numberNewFaces);

    int& oldNumFaces = polyhedron.NumFaces;
    // int& oldNumEdges = polyhedron.NumEdges;

    // Forse non ci serve: provo a scrivere tutto senza usarlo
    /* Let's create a new polyhedron in order to save the data structures of the tessellated polyhedron */
    //GEOPolyhedron tessellatedPolyhedron;

    /* We need to tesselate each face of the polyhedron given as input */
    for(int faceIndex = 0; faceIndex < oldNumFaces; faceIndex++)
    {
        // Investigare il bug che c'è nella successiva riga a questo commento
        // Eigen::VectorXi& faceVertices = ListVertFaces.col(faceIndex);

        /* In order to tesselate, we need to divide each edge into "numberDivisions" parts */
        for(int edgeIndex = 0; edgeIndex < p; edgeIndex++)
        {
            /* We need to remember that the edge with "edgeIndex = 2" has the vertices 
            with index 2 and 0 (not 2 and 3) */
            Eigen::Vector<int, 2> verticesWeAreDividing;
            if(edgeIndex == 2)
            {
                /* Let's save the vertices of the edge which we'll need to calculate 
                the length of the new smaller edges */
                verticesWeAreDividing(0) = ListVertFaces(edgeIndex, faceIndex);
                verticesWeAreDividing(1) = ListVertFaces(0, faceIndex);
            }
            else
            {
                verticesWeAreDividing(0) = ListVertFaces(edgeIndex, faceIndex);
                verticesWeAreDividing(1) = ListVertFaces(edgeIndex + 1, faceIndex);

            }
                /* Let's rename the vertices of the edge for code readability */
                int& firstVertexWeAreDividing = verticesWeAreDividing(0);
                int& secondVertexWeAreDividing = verticesWeAreDividing(1);

                /* Let's save the coordinates of the two vertices */
                double& firstVertexXCoord = CoordVertices(0, firstVertexWeAreDividing);
                double& firstVertexYCoord = CoordVertices(1, firstVertexWeAreDividing);
                double& firstVertexZCoord = CoordVertices(2, firstVertexWeAreDividing);

                double& secondVertexXCoord = CoordVertices(0, secondVertexWeAreDividing);
                double& secondVertexYCoord = CoordVertices(1, secondVertexWeAreDividing);
                double& secondVertexZCoord = CoordVertices(2, secondVertexWeAreDividing);

                /* We'll add the length of the new smaller edge to the minimum vertex between the two, 
                so let's find its coordinates */
                double& vertexWeAreStartingFromXCoord = min(firstVertexXCoord, secondVertexXCoord);
                double& vertexWeAreStartingFromYCoord = min(firstVertexYCoord, secondVertexYCoord);
                double& vertexWeAreStartingFromZCoord = min(firstVertexZCoord, secondVertexZCoord);

                double differenceXCoord = abs(firstVertexXCoord - secondVertexXCoord);
                double differenceYCoord = abs(firstVertexYCoord - secondVertexYCoord);
                double differenceZCoord = abs(firstVertexZCoord - secondVertexZCoord);

                /* Let's add the coordinates of the new points on each edge to the matrix 
                "CoordVertices" of the polyhedron we're modifying */
                for(int division = 1; division < numberDivisions; division++)
                {
                    /* The new vertices will be stored inside the same matrix CoordVertices, 
                    so we need to access the correct column in order not to erase important data 
                    from the old polyhedron */
                    CoordVertices(0, NumVertices + edgeIndex + division) = 
                        vertexWeAreStartingFromXCoord + differenceXCoord / numberDivisions;
                    CoordVertices(1, NumVertices + edgeIndex + division) = 
                        vertexWeAreStartingFromYCoord + differenceYCoord / numberDivisions;
                    CoordVertices(2, NumVertices + edgeIndex + division) = 
                        vertexWeAreStartingFromZCoord + differenceZCoord / numberDivisions;
                }
        }
        
        /* Now we need to find the coordinates of the vertices inside of the face of the 
        old polyhedron, then we'll look for the edges starting from each vertex using the 
        same algorithm we used to build the old polyhedron, using the length of the edges */

        /* Io vorrei prendere i vertici che stanno sulla stessa retta orizzontale nei disegni a pagina 3 di 
        PCS_project.pdf e dividere la loro distanza in modo equivalente come ho fatto sopra per ciascun lato 
        della faccia. Salendo verso il vertice in alto si può notare che i segmenti orizzontali vengono divisi 
        in parti uguali prima b volte, poi b-1 volte, poi b-2 volte e così via fino a finire nel segmento 
        che non viene diviso. Non credo che il modo per trovare i lati che ho scritto sopra e che usa il mio 
        algoritmo sia efficiente, ma è l'unica via che mi viene in mente. */

    }
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
