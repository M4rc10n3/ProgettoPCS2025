/* File containing the name and a little description of the functions we'll use */

# pragma once
# include "Polyhedra.hpp"

using namespace PolyhedraLibrary;

/* CreatePolyhedron is a function that creates the Polyhedron starting from the input data p and q
Inputs list:
p: number of vertices of each face of the polyhedron; therefore, each face is a p-sided polygon
q: number of regular p-sided polygon faces around each vertex of the polyhedra*/
Polyhedron CreatePolyhedron(int& p, int& q);

/* TypeITassellation is a function that modifies the attributes of the object <polyhedron> in order to 
output its tassellations of type I regarding geodetic polyhedra 
Inputs list:
polyhedron: object of type "Polyhedron" that the function modifies. */
void TypeITassellation(Polyhedron& polyhedron);

/* TypeIITassellation is a function that modifies the attributes of the object <polyhedron> in order to 
output its tassellations of type II regarding geodetic polyhedra 
Inputs list:
polyhedron: object of type "Polyhedron" that the function modifies. */
void TypeIITassellation(Polyhedron& polyhedron);

/* Dualise is a function that outputs the dual, expressed as the type "Polyhedron", of a <polyhedron> given as input 
Inputs list:
polyhedron: object of type "Polyhedron" that the function uses to create its dual */
Polyhedron Dualise(Polyhedron& polyhedron);
// Che sia meglio dare come output il tipo void? Se facciamo return Polyhedron sarà una grossa e lenta operazione, vero? 
// Forse è meglio definire un nuovo poliedro prima di chiamare questa funzione e mettiamo come altro input della funzione il poliedro stesso in cui dobbiamo fare output?

/* OntoTheSphere is a function that takes a point as input and projects it onto the unitary sphere 
Inputs list:
vertex: the point that the function will projects onto the sphere normalising its vector*/
void OntoTheSphere(vertex& vertex);
// Pensavo di usare la normalizzazione per proiettare sulla sfera unitaria, a voi viene in mente un modo migliore? Pensate che il mio funzioni?
// La variabile vertex ha come tipo vertex, ma in realtà non so ancora cosa sarà, probabilmente un vettore.

/* WhichIsTheMinimumPathBetween is a function that calculates the minimum path between the two vertices marked 
by the ids given as inputs 
Inputs list:
id_vertex_1: the id associated to the first vertex of the minimum path we're looking for 
id_vertex_2: the id associated to the second/last vertex of the minimum path we're looking for*/
Path WhichIsTheMinimumPathBetween(int& id_vertex_1, int& id_vertex_2);