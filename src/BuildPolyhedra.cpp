# include <iostream>
# include <fstream>
# include <vector>
# include <set>
# include <math.h>
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

        polyhedron.IdVertices.reserve(NumVertices);
        polyhedron.IdEdges.reserve(NumEdges);
        polyhedron.IdFaces.reserve(NumFaces);

        polyhedron.CoordVertices = Eigen::MatrixXd(NumVertices, 3);
        polyhedron.ExtremaEdges = Eigen::MatrixXi(NumEdges, 2);
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
                    polyhedron.CoordVertices << 0,0,1,
                                                -0.94,0,-0.33,
                                                0.47,-0.82,-0.33,
                                                0.47,0.82,-0.33;
                    break;
                case 4:
                    cout << "Your Polyhedron is a Octahedron with:\n";
                    Length_edge = sqrt(2);
                    polyhedron.CoordVertices << 0,0,1,
                                                0,0,-1,
                                                0,1,0,
                                                0,-1,0,
                                                1,0,0,
                                                -1,0,0;
                    break;
                case 5:
                    cout << "Your Polyhedron is a Icosahedron with: \n";
                    Length_edge = 4 / sqrt(10 + 2*sqrt(5));
                    polyhedron.CoordVertices << 0,0,1,
                                                0.89,0,0.45,
                                                0.28,0.85,0.45,
                                                0.72,0.53,-0.45,
                                                -0.28,0.85,-0.45,
                                                0,0,-1,
                                                -0.89,0,-0.45,
                                                -0.28,-0.85,-0.45,
                                                -0.72,-0.53,0.45,
                                                0.28,-0.85,0.45,
                                                0.72,-0.53,-0.45,
                                                -0.72,0.53,0.45;
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
        Eigen::MatrixXd& CoordVertices = polyhedron.CoordVertices;
        vector<double> x(NumVertices), y(NumVertices), z(NumVertices);
        vector<int> vert_valence(NumVertices);
        
        double angle_vectors;

        x[0] = 0;
        y[0] = 0;
        z[0] = 1;
        
        // Piuttosto di "pow" la prof. aveva detto che per potenze piccole è meglio 
        // moltiplicare l'elemento per se stesso
        x[1] = sqrt(1 - pow(0.5*(2.0 - Length_edge * Length_edge), 2));
        y[1] = 0;
        z[1] = 0.5*(2.0 - Length_edge * Length_edge);

        angle_vectors = x[0] * x[1] + y[0] * y[1] + z[0] * z[1];
        // cout << angle_vectors << endl;
        // cout << x[1] * x[1] + y[1] * y [1] + z[1] * z[1];
        for (int i = 2; i < NumVertices; i++)
        {

        }
    }

    void BuildPolyhedra::FillStructPolyhedra()
    {   
        PointsPolyhedra();            
        Eigen::MatrixXi& ExtremaEdges = polyhedron.ExtremaEdges;
        Eigen::MatrixXi& MatrEdgeVertices = polyhedron.MatrEdgeVertices;
        Eigen::MatrixXi& ListVertFaces = polyhedron.ListVertFaces;
        Eigen::MatrixXi& ListEdgeFaces = polyhedron.ListEdgeFaces;

        unsigned int edgeIndex = 0;
        // A cosa serviva questa variabile?
        // double& length_edge = Length_edge;
        double length_edge_squared = Length_edge * Length_edge;

        for(int i = 0; i < NumVertices - 1; i++)
        {
            /* Non c'è sequenzialità nel dare gli estremi dei vertici, perchè, per esempio nell'ottaedro lo 0 va conesso ai suoi 3 prossimi, 
                ma il vertice con id 1  va connesso ai vertici con id 2, 4 e 5, non connessi da alcuna logica. Quindi tocca unire i vertici che hanno la giusta distanza, 
                ovvero la lunghezza del lato */
            
            // Saving in some variables the coordinates of the first vertex
            double x_point_1 = polyhedron.CoordVertices(i, 0);
            double y_point_1 = polyhedron.CoordVertices(i, 1);
            double z_point_1 = polyhedron.CoordVertices(i, 2);

            for (int j = i + 1; j < NumVertices; j++)
            {
                // Saving in some variables the coordinates of the second vertex
                double x_point_2 = polyhedron.CoordVertices(j, 0);
                double y_point_2 = polyhedron.CoordVertices(j, 1);
                double z_point_2 = polyhedron.CoordVertices(j, 2);

                // Calculating the distance between the two vertices, but squared
                double distance_squared = (x_point_1 - x_point_2) * (x_point_1 - x_point_2) + 
                                          (y_point_1 - y_point_2) * (y_point_1 - y_point_2) +
                                          (z_point_1 - z_point_2) * (z_point_1 - z_point_2);

                // When the two vertices have the correct distance between them we save them as an 
                // edge of the polyhedron
                if(distance_squared - length_edge_squared < 5e-2)
                {
                    ExtremaEdges(edgeIndex,0) = i;
                    ExtremaEdges(edgeIndex,1) = j;

                    // Se un lato ha id 0 allora non potremo mai vederlo in una matrice sparsa, 
                    // quindi la matrice è inizializzata con tutti valori pari a -1
                    // A cosa serve questa matrice esattamente?
                    // In questo momento al posto (i,j) sta l'id del lato che connette i vertici con id i e id j
                    MatrEdgeVertices(i,j) = edgeIndex;
                    MatrEdgeVertices(j,i) = edgeIndex;

                    // Passing to the next edge only if we saved an edge during this iteration
                    edgeIndex++;
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


        unsigned int faceIndex = 0;

        // Modo di Marco per salvare le facce perché purtroppo non ho capito il codice che è stato scritto

        // for(int vertex = 0; vertex < NumVertices; vertex++)
        // {
        //     // Saving the vertices that create an edge together with "vertex"
        //     vector<int> verticesConnectedToVertex;

        //     for(int element = 0; element < NumVertices; element++)
        //     {
        //         int& edgeIdToCheck = MatrEdgeVertices(vertex, element);
        //         if(edgeIdToCheck >= 0){
        //             verticesConnectedToVertex.push_back(element);
        //         }
        //     }
        //     // Stampa per controllare che abbia salvato i vertici corretti
        //     cout << "verticesConnectedToVertex: { ";
        //     for(auto elem : verticesConnectedToVertex)
        //     {
        //         cout << elem << " ";
        //     }
        //     cout << "}" << endl;
        //     // Salva i vertici corretti

        //     // Ora dobbiamo salvare il secondo e terzo vertice della faccia e vedere se essi formano un lato o meno;
        //     // se così è, allora abbiamo trovato una faccia.

        //     for(int i = 0; i <= verticesConnectedToVertex.size(); i++){
        //         int vertexToCheck1 = verticesConnectedToVertex[i];
        //         cout << "i: " << i << endl;
        //         for(int j = i + 1; j <= verticesConnectedToVertex.size(); j++ ){
        //             int vertexToCheck2 = verticesConnectedToVertex[j];
        //             cout << "j: " << j << endl;
        //             // TODO: Ci va un controllo sul fatto che la faccia non sia già stata salvata e sul fatto che 
        //             // il programma salvi anche le facce che hanno 2 vertici in comune
                    
        //             // Al momento questo if presenta un bug che salva le facce con i vertici 0, 2, 0, 
        //             // oppure 0, 4, 0 che ovviamente non formano una faccia
        //             if(MatrEdgeVertices(vertexToCheck1,vertexToCheck2) >= 0){
        //                 // Abbiamo trovato i tre vertici che formano una faccia
        //                 cout << "There is an edge with vertices: ( " << i << ", " << j << " )" << endl;
        //                 ListVertFaces(0, faceIndex) = vertex;
        //                 ListVertFaces(1, faceIndex) = vertexToCheck1;
        //                 ListVertFaces(2, faceIndex) = vertexToCheck2;
                        
        //                 // Stampa per controllo
        //                 cout << "ListVertFaces: " << endl << ListVertFaces << endl;

        //                 // Bisogna salvare i lati della faccia nella matrice polyhedron.ListEdgeFaces
                        
                        
        //                 // Passing to the next face only if we saved a face during this iteration
        //                 faceIndex++;
        //             }
        //             else{
        //                 cout << "There isn't an edge with vertices: ( " << i << ", " << j << " )" << endl;
        //             }
        //         }
        //     }                      
        // }
         
        for(int vertex = 0; vertex < NumVertices; vertex++)
        {
            // Saving the vertices that create an edge together with "vertex"
            vector<int> verticesConnectedToVertex;
            int element = 0;
            while(element < NumVertices && verticesConnectedToVertex.size() < = q)
            {
                int& edgeIdToCheck = MatrEdgeVertices(vertex, element);
                if(edgeIdToCheck >= 0){
                    verticesConnectedToVertex.push_back(element);
                    element++;
                }
            }
            // Stampa per controllare che abbia salvato i vertici corretti
            cout << "verticesConnectedToVertex: { ";
            for(auto elem : verticesConnectedToVertex)
            {
                cout << elem << " ";
            }
            cout << "}" << endl;
            // Salva i vertici corretti

            // Ora dobbiamo salvare il secondo e terzo vertice della faccia e vedere se essi formano un lato o meno;
            // se così è, allora abbiamo trovato una faccia.

            for(int i = 0; i <= verticesConnectedToVertex.size(); i++){
                int vertexToCheck1 = verticesConnectedToVertex[i];
                cout << "i: " << i << endl;
                for(int j = i + 1; j <= verticesConnectedToVertex.size(); j++ ){
                    int vertexToCheck2 = verticesConnectedToVertex[j];
                    cout << "j: " << j << endl;
                    // TODO: Ci va un controllo sul fatto che la faccia non sia già stata salvata e sul fatto che 
                    // il programma salvi anche le facce che hanno 2 vertici in comune
                    
                    // Al momento questo if presenta un bug che salva le facce con i vertici 0, 2, 0, 
                    // oppure 0, 4, 0 che ovviamente non formano una faccia
                    if(MatrEdgeVertices(vertexToCheck1,vertexToCheck2) >= 0){
                        // Abbiamo trovato i tre vertici che formano una faccia
                        cout << "There is an edge with vertices: ( " << i << ", " << j << " )" << endl;
                        ListVertFaces(0, faceIndex) = vertex;
                        ListVertFaces(1, faceIndex) = vertexToCheck1;
                        ListVertFaces(2, faceIndex) = vertexToCheck2;
                        
                        // Stampa per controllo
                        cout << "ListVertFaces: " << endl << ListVertFaces << endl;

                        // Bisogna salvare i lati della faccia nella matrice polyhedron.ListEdgeFaces
                        
                        
                        // Passing to the next face only if we saved a face during this iteration
                        faceIndex++;

                        vecVertFaces.push_back(vertFaces);
                    }
                    else{
                        cout << "There isn't an edge with vertices: ( " << i << ", " << j << " )" << endl;
                    }
                }
            }                      
        }

        // Questo è il vettore dei vettori di una faccia?
        vector<int> adjVert;

        //O è questo quello dei vertici di una faccia?
        set<int> vertFaces;
        vector<set<int>> vecVertFaces;

        int v1, v2, v3, j;

        for(int i = 0; i < NumVertices; i++)
        {
            adjVert.clear();
            j = 0;
            while (j < NumVertices && adjVert.size() <= q)
            {
                if(MatrEdgeVertices(i,j) >= 0 && adjVert.size() < q)
                {
                    adjVert.push_back(j);
                }
                j++;
            }
            for (unsigned int k = 1; k < adjVert.size(); k++)
            {
                v1 = adjVert[k-1];
                v2 = adjVert[k];

                if(MatrEdgeVertices(v1, v2) >= 0)
                {
                    vertFaces = {i, v1, v2};
                    
                    if(find(vecVertFaces.begin(), vecVertFaces.end(), vertFaces) == vecVertFaces.end())
                    {
                        polyhedron.ListVertFaces(0, faceIndex) = i;
                        polyhedron.ListVertFaces(1, faceIndex) = v1;
                        polyhedron.ListVertFaces(2, faceIndex) = v2;
                        faceIndex++;
                        
                    }
                }
            }
        }

        faceIndex = 0;
        
        int vert[3];

        for (set<int> vertFaces : vecVertFaces)
        {
            // alternativa 1
            // auto it = vertFaces.begin();
            // v1 = *it++;
            // v2 = *it++;
            // v3 = *it;

            // alternativa 2
            // copy(vertFaces.begin(), vertFaces.end(), vert);
            
            // alternativa 3
            // unsigned int index = 0;

            // for (int val : vertFaces)
            // {
            //     vert[index] = val;
            //     index++;
            // }

            v1 = vert[0];
            v2 = vert[1];
            v3 = vert[2];

            ListEdgeFaces(0, faceIndex) = MatrEdgeVertices(v1,v2);
            ListEdgeFaces(1, faceIndex) = MatrEdgeVertices(v2,v3);
            ListEdgeFaces(2, faceIndex) = MatrEdgeVertices(v1,v3);
            faceIndex++;
        }

    }

    void BuildPolyhedra::Cell0Ds()
    {   
        Eigen::MatrixXd& CoordVertices = polyhedron.CoordVertices;
        // Eigen::MatrixXd CoordVertices = Eigen::MatrixXd::Zero(NumVertices, 3);
        
        ofstream file("../PolygonalData/Cell0Ds.txt"); // the program should be launched inside Debug or Release folders
        
        file << "Id,X,Y,Z\n";
        for (int i = 0; i < NumVertices; i++)
        {
            file << i << "," << 
            CoordVertices(i, 0) << "," << 
            CoordVertices(i, 1) << "," << 
            CoordVertices(i, 2) << "\n";
            
            polyhedron.IdVertices.push_back("V" + to_string(i));
        }

        file.close();
    }

    void BuildPolyhedra::Cell1Ds()
    {   
        Eigen::MatrixXi ExtremaEdges = polyhedron.ExtremaEdges;
        // Eigen::MatrixXd ExtremaEdges = Eigen::MatrixXd::Zero(NumEdges, 2);
        ofstream file("../PolygonalData/Cell1Ds.txt"); // the program should be launched inside Debug or Release folders
        
        file << "Id,Origin,End\n";
        for (int i = 0; i < NumEdges; i++)
        {
            file << i << "," << ExtremaEdges(i,0) << "," << 
            ExtremaEdges(i,1) << "\n";
            
            polyhedron.IdEdges.push_back("E" + to_string(i));
        }

        file.close();
    }

    void BuildPolyhedra::Cell2Ds()
    {   
        Eigen::MatrixXi ListVertFaces = polyhedron.ListVertFaces;
        Eigen::MatrixXi ListEdgeFaces = polyhedron.ListEdgeFaces;
        // Eigen::MatrixXi ListVertFaces = Eigen::MatrixXi::Zero(NumVertices, NumFaces);
        // Eigen::MatrixXi ListEdgeFaces = Eigen::MatrixXi::Zero(NumEdges, NumFaces);
        ofstream file("../PolygonalData/Cell2Ds.txt"); // the program needs to be launched inside Debug or Release folders
        
        file << "Id,NumVerices,Vertices,NumEdges,Edges\n";
        for (int i = 0; i < NumFaces; i++)
        {
            file << i << "," << p;
            for (int j = 0; j < p; j++)
            {
                file << "," << ListVertFaces(j, i);
            }
            
            file << "," << p;
            for (int k = 0; k < p; k++)
            {
                file << "," << ListEdgeFaces(k, i);
            }
            file << "\n";

            polyhedron.IdFaces.push_back("F" + to_string(i));
        }

        file.close();
    }

    void BuildPolyhedra::Cell3Ds()
    {   
        ofstream file("../PolygonalData/Cell3Ds.txt"); // the program should be launched inside Debug or Release folders
        
        file << "Id\n";
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
