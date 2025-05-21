# include <iostream>
# include <fstream>
# include <vector>
# include <set>
# include <math.h>
# include <algorithm>
# include "Eigen/Eigen"
# include "Polyhedra.hpp"
# include "BuildPolyhedra.hpp"

using namespace std;

namespace PolyhedraLibrary{
    BuildPolyhedra::BuildPolyhedra(const int& Schlafli_p, const int& Schlafli_q)
    {
        p = Schlafli_p;
        q = Schlafli_q;

        polyhedron.NumFaces = (4 * q) / ((2*p) - (p*q) + 2*q); // determines the number of faces using p and q 
        polyhedron.NumEdges = (p * polyhedron.NumFaces) / 2;
        polyhedron.NumVertices = (p * polyhedron.NumFaces) / q;

        // Così stiamo creando delle copie, non sarebbe meglio creare dei semplici alias? 
        // Per farlo si usa la sintassi:
        // int &NumFaces = polyhedron.NumFaces;
        NumFaces = polyhedron.NumFaces; 
        NumEdges = polyhedron.NumEdges;
        NumVertices = polyhedron.NumVertices;

        // Credo sia utile se rendiamo gli id come int o unsigned int piuttosto che string
        // polyhedron.IdVertices.reserve(NumVertices);
        // polyhedron.IdEdges.reserve(NumEdges);
        // polyhedron.IdFaces.reserve(NumFaces);

        polyhedron.CoordVertices = Eigen::MatrixXd(3, NumVertices);
        polyhedron.ExtremaEdges = Eigen::MatrixXi(2, NumEdges);
        polyhedron.MatrEdgeVertices = Eigen::MatrixXi::Constant(NumVertices, NumVertices, -1);
        polyhedron.ListEdgeFaces = Eigen::MatrixXi(p, NumFaces);
        polyhedron.ListVertFaces = Eigen::MatrixXi(p, NumFaces);
        
    }

    void BuildPolyhedra::DataPolyhedra()
    {
        cout << "Entering DataPolyhedra" << endl;
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
                    polyhedron.CoordVertices << 0, -0.94, 0.47, 0.47,
                                                0, 0, -0.82, 0.82,
                                                1, -0.33, -0.33, -0.33;
                    break;
                case 4:
                    cout << "Your Polyhedron is a Octahedron with:\n";
                    Length_edge = sqrt(2);
                    polyhedron.CoordVertices << 0, 0, 0, 0, 1, -1,
                                                0, 0, 1, -1, 0, 0,
                                                1, -1, 0, 0, 0, 0;
                    break;
                case 5:
                    cout << "Your Polyhedron is a Icosahedron with: \n";
                    Length_edge = 4 / sqrt(10 + 2*sqrt(5));
                    polyhedron.CoordVertices << 0, 0.89, 0.28, 0.72, -0.28, 0, -0.89, -0.28, -0.72, 0.28, 0.72, -0.72,
                                                0, 0, 0.85, 0.53, 0.85, 0, 0, -0.85, -0.53, -0.85, -0.53, 0.53,
                                                1, 0.45, 0.45, -0.45, -0.45, -1, -0.45, -0.45, 0.45, 0.45, -0.45, 0.45;   
                    break;
                }
                break;
            case 4:
                if (q == 3)
                {
                    cout << "Your Polyhedron is a Cube with: \n";
                    Length_edge = 2 / sqrt(3);
                }
                break;
            case 5:
                if (q == 3)
                {
                    cout << "Your Polyhedron is a Dodecahedron with: \n";
                    Length_edge = 4 / (sqrt(3) * (1 + sqrt(5)));
                }
                break;
            }
            cout << NumVertices << " Vertices\n" <<
                    NumEdges << " Edges\n" <<
                    NumFaces << " Faces\n" << endl;
        }
        // Questo else da me aggiunto serve?
        else
        {
            cout << "This program cannot handle your polyhedron."  << endl;
            cout << "This program only works with platonic polyhedra."  << endl;
        }
    }

    void BuildPolyhedra::PointsPolyhedra()
    {   
        DataPolyhedra();
        // Eigen::MatrixXd& CoordVertices = polyhedron.CoordVertices;
        // vector<double> x(NumVertices), y(NumVertices), z(NumVertices);
        // vector<int> vert_valence(NumVertices);
        
        // double angle_vectors;

        // x[0] = 0;
        // y[0] = 0;
        // z[0] = 1;
        
        // // Piuttosto di "pow" la prof. aveva detto che per potenze piccole è meglio 
        // // moltiplicare l'elemento per se stesso
        // x[1] = sqrt(1 - pow(0.5*(2.0 - Length_edge * Length_edge), 2));
        // y[1] = 0;
        // z[1] = 0.5*(2.0 - Length_edge * Length_edge);

        // angle_vectors = x[0] * x[1] + y[0] * y[1] + z[0] * z[1];
        // // cout << angle_vectors << endl;
        // // cout << x[1] * x[1] + y[1] * y [1] + z[1] * z[1];
        // for (int i = 2; i < NumVertices; i++)
        // {

        // }
    }

    void BuildPolyhedra::FillStructPolyhedra()
    {   
        PointsPolyhedra();            
        Eigen::MatrixXi& ExtremaEdges = polyhedron.ExtremaEdges;
        Eigen::MatrixXi& MatrEdgeVertices = polyhedron.MatrEdgeVertices;
        Eigen::MatrixXi& ListVertFaces = polyhedron.ListVertFaces;
        Eigen::MatrixXi& ListEdgeFaces = polyhedron.ListEdgeFaces;
        
        /* Creating the matrix containing the vertices of each edge and the matrix with the ids 
        of each edge at the coordinates i and j, where i and j are its two vertices.
        We do it by checking the distance between a vertex and all of the others, by using a for cycle 
        that doesn't check for the last two vertices (that iteration would be useless) */
        int edgeIndex = 0;
        double length_edge_squared = Length_edge * Length_edge;

        for(int i = 0; i < NumVertices - 1; i++)
        {
            if(edgeIndex <= NumEdges) // Proceed only if all the edges have not been numbered yet
            {
                /* Non c'è sequenzialità nel dare gli estremi dei vertici, perchè, per esempio nell'ottaedro lo 0 va conesso ai suoi 3 prossimi, 
                    ma il vertice con id 1  va connesso ai vertici con id 2, 4 e 5, non connessi da alcuna logica. Quindi tocca unire i vertici che hanno la giusta distanza, 
                    ovvero la lunghezza del lato */
                
                // Saving in some variables the coordinates of the first vertex
                double& x_point_1 = polyhedron.CoordVertices(0, i);
                double& y_point_1 = polyhedron.CoordVertices(1, i);
                double& z_point_1 = polyhedron.CoordVertices(2, i);

                for (int j = i + 1; j < NumVertices; j++)
                {
                    // Saving in some variables the coordinates of the second vertex
                    double& x_point_2 = polyhedron.CoordVertices(0, j);
                    double& y_point_2 = polyhedron.CoordVertices(1, j);
                    double& z_point_2 = polyhedron.CoordVertices(2, j);

                    // Calculating the distance(squared) between the two vertices
                    double distance_squared = (x_point_1 - x_point_2) * (x_point_1 - x_point_2) + 
                                                (y_point_1 - y_point_2) * (y_point_1 - y_point_2) +
                                                (z_point_1 - z_point_2) * (z_point_1 - z_point_2);

                    // When the two vertices have the correct distance between them we save them as an 
                    // edge of the polyhedron (we decided to set this small tolerance because the data 
                    // we used to create the polygons has two decimal digits)
                    if(distance_squared - length_edge_squared < 5e-2)
                    {
                        ExtremaEdges(0, edgeIndex) = i;
                        ExtremaEdges(1, edgeIndex) = j;

                        // Se un lato ha id 0 allora non potremo mai vederlo in una matrice sparsa, 
                        // quindi la matrice è inizializzata con tutti valori pari a -1
                        MatrEdgeVertices(i,j) = edgeIndex;
                        MatrEdgeVertices(j,i) = edgeIndex;

                        // Passing to the next edge only if we saved an edge during this iteration
                        edgeIndex++;
                    }
                }
            }
        }
        
        cout << "ExtremaEdges: " << endl << ExtremaEdges << endl;
        cout << "MatrEdgeVertices: " << endl << MatrEdgeVertices << endl;
        
        // for(int i = 0; i < NumEdges; i++){
        //     int a = ExtremaEdges[i, 0];
        //     int b = ExtremaEdges[i, 1];
        //     MatrEdgeVertices(a,b) = edgeIndex;
        //     MatrEdgeVertices(b,a) = edgeIndex;
        //     edgeIndex++;
        // }

        /* Creating the matrix containing the vertices of each face as its column and the matrix containing the ids 
        of the edges of each face. */

        int faceIndex = 0;

        vector<array<int, 3>> vecVertFaces; // This vector stores unique triangles (faces) as sorted arrays of 3 vertices
        vecVertFaces.reserve(NumFaces);
        
        for(int vertex = 0; vertex < NumVertices; vertex++)
        { 
            if (faceIndex <= NumFaces) // Proceed only if all the faces have not been numbered yet
            {
                // Saving the vertices that are adjacent to "vertex"
                vector<int> connectedVertices;
                connectedVertices.reserve(q);

                int element = 0;
                while(element < NumVertices && int(connectedVertices.size()) < q)
                {
                    int& edgeIdToCheck = MatrEdgeVertices(vertex, element);
                    if(edgeIdToCheck >= 0){
                        connectedVertices.push_back(element);
                    }
                    element++;
                }

                // Try forming triangles from combinations of connected vertices
                for(int i = 0; i < int(connectedVertices.size()); i++)
                {
                    int& vertexToCheck1 = connectedVertices[i];
                    // cout << "i: " << i << endl;
                    for(int j = i + 1; j < int(connectedVertices.size()); j++)
                    {
                        int& vertexToCheck2 = connectedVertices[j];
                        // cout << "j: " << j << endl;
                        
                        // Check all three vertices are distinct
                        if(vertex != vertexToCheck1 && vertex != vertexToCheck2 && vertexToCheck1 != vertexToCheck2)
                        {                    
                            int& edgeIdToAdd = MatrEdgeVertices(vertexToCheck1, vertexToCheck2);     
                            if (edgeIdToAdd >= 0) // Proceed only if there is an edge that connects the two vertices
                            {
                                array<int, 3> sortedVertFace = {vertex, vertexToCheck1, vertexToCheck2};
                                sort(sortedVertFace.begin(), sortedVertFace.end()); // Sorting avoids counting multiple times the same triangles with different vertex ordering

                                // Check if the sorted triangle is already in the vector

                                // Come funziona la funzione find?

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



    void BuildPolyhedra::Cell0Ds()
    {   
        Eigen::MatrixXd& CoordVertices = polyhedron.CoordVertices;
        
        ofstream file("../PolygonalData/Cell0Ds.txt"); // the program should be launched inside Debug or Release folders
        
        file << "Id,X,Y,Z\n";
        for (int i = 0; i < NumVertices; i++)
        {
            file << i << "," << 
            CoordVertices(0, i) << "," << 
            CoordVertices(1, i) << "," << 
            CoordVertices(2, i) << "\n";
            
            polyhedron.IdVertices.push_back("V" + to_string(i));
        }

        file.close();
    }

    void BuildPolyhedra::Cell1Ds()
    {   
        Eigen::MatrixXi ExtremaEdges = polyhedron.ExtremaEdges;

        ofstream file("../PolygonalData/Cell1Ds.txt"); // the program should be launched inside Debug or Release folders
        
        file << "Id,Origin,End\n";
        for (int i = 0; i < NumEdges; i++)
        {
            file << i << "," << ExtremaEdges(0, i) << "," << 
            ExtremaEdges(1, i) << "\n";
            
            polyhedron.IdEdges.push_back("E" + to_string(i));
        }

        file.close();
    }

    void BuildPolyhedra::Cell2Ds()
    {   
        Eigen::MatrixXi& ListVertFaces = polyhedron.ListVertFaces;
        Eigen::MatrixXi& ListEdgeFaces = polyhedron.ListEdgeFaces;
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

            polyhedron.IdFaces.push_back("F" + to_string(i));
        }

        file.close();
    }

    void BuildPolyhedra::Cell3Ds()
    {   
        ofstream file("../PolygonalData/Cell3Ds.txt"); // the program should be launched inside Debug or Release folders
        
        file << "IdVertices:\n";
        for (int i = 0; i < NumVertices; i++)
        {
            file << polyhedron.IdVertices[i] << "\n";
        }

        for (int j = 0; j < NumEdges; j++)
        {
            file << polyhedron.IdEdges[j] << "\n";
        }

        for (int k = 0; k < NumFaces; k++)
        {
            file << polyhedron.IdFaces[k] << "\n";
        }

        file.close();
    }

    void BuildPolyhedra::CreateCells() 
    {
        FillStructPolyhedra();

        Cell0Ds();

        Cell1Ds();

        Cell2Ds();

        Cell3Ds();
    }


};
