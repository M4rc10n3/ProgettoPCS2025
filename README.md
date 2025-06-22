# ProgettoPCS2025

Code and documentation for the PCS project 2025

Domande:
1. Il file .txt può essere un .csv oppure un file che ha degli spazi come delimitatori? 

   Risposta: possiamo formattarli come vogliamo, basta che l'estensione sia .txt


2. Dobbiamo solo fare in modo che il programma dia output solo per poliedri che hanno p = 3, così come dia il poliedro di Goldberg solo per quelli con q = 3 ?

   Risposta: conferma. Per ottenere i poliedri di Goldberg per q=3 (partendo dal cubo e dodecaedro) si passa dai poliedri con p=3 (tetraedro, ottaedro, icosaedro), quindi sono questi ultimi quelli su cui dobbiamo concentrarci. Se ci dessero come _input_ {4, 3}, possiamo partire da {3, 4} e svolgere ciò che ci è richiesto.


3. Come possiamo ottenere il _dataset_ dei vertici dei poliedri di partenza?

   Risposta: alla prof. non interessa come arriviamo ai vertici di partenza, ma consiglia di usare carta e penna, perché così i vertici a cui arriviamo sono corretti e il costo computazionale del codice diventa minore. Con il nostro algoritmo si ottiene un programma più generalizzato, quindi consiglia poi di farlo alla fine di tutto il programma.


4. Come si ottiene la tassellazione delle facce che rappresenta il primo passo per trovare il poliedro geodetico? 

   Risposta: pare che non esistano algoritmi online per spiegare i casi più semplici di tassellazione (ci sono solo per $b!=c$ che non ci interessa per il progetto). La prima tassellazione è basata sul fatto che dividiamo per b ciascun lato (in modo da ottenere una divisione equispaziata), così da ottenere i vertici esterni che poi vanno connessi tra loro. I vertici interni possono essere ottenuti come intersezioni dei lati interni alle facce così ottenuti oppure come divisione equispaziata dei segmenti paralleli ai lati (a voce ve lo potrò spiegare meglio, altrimenti chiedete pure alla Teora). 
   La seconda tasselazione, invece, si può ottenere a partire dalla prima trovando i baricentri di tutte le facce trovate con la prima tassellazione e poi unirli con i baricentri delle facce adiacenti, se esistono, oppure con i punti medi dei lati della faccia a cui appartiene il baricentro.


5. La triangolazione di tipo II non deve avere i triangoli rossi, vero? 

   Risposta: i triangoli rossi nei disegni servono solo come punto di partenza per la tassellazione di tipo II, ma non fanno parte di quest'ultima.

6. Cosa indica la formula 4?
   Risposta: la prima formula delle due indica che, se la faccia ha 3 vertici, allora il vertice a cui si accede all'interno del vettore dei vertici tramite l'indice zero si connetterà a quello a cui si accede tramite indice 1, quello a cui si accede con indice 1 si connetterà a quello a cui si accede con indice 2, mentre quello con indice 2 si connetterà a quello con indice 0, che è (2 + 1) % 3 = 0.
   La seconda formula, invece, dice che il vertice della faccia a cui si accede nella lista/vettore tramite l'indice 0 deve essere l'origine (o la fine) del lato della faccia a cui si accede nella lista/vettore tramite l'indice 0.

7. Le formule che danno vertici, lati e facce per i duali dei poliedri valgono per ciascuna faccia o per tutto il poliedro?
Tali formule valgono solo per i poliedri di tipo I, mentre quella di tipo II vale solo se consideriamo il contributo delle varie aree e delle lunghezze: ci sono triangoli con area pari a 1/2 di quella di un triangolo.

Il caso in cui p = q = 3 non è detto come gestirlo, si può gestire come si vuole (magari chiedendo all'utente di specificare quale desidera)

8: Formule per il numero di componenti del secondo tipo di tassellazione? 
Risposta:
 V = numV + numE*(2b-1)+numF((3b^2)/2-3b/2+1)
 E = numE(2b)+numF((9b^2)/2+3b/2)
 F = numF((3b^2)+3b)

 Relazione di Eulero: 
 V+F=E+2
 
 numV,numE,numF del solido platonico di partenza

Cose da fare:
1. Creare algoritmo che crea il poliedro a partire da p e q;
2. Creare algoritmo che ne fa la tassellazione a seconda della classe;
   - Creare una funzione che calcola i baricentri dei poligoni che sono facce del poliedro.
3. Creare algoritmo che fa il duale di tale tassellazione;
4. Creare una funzione che proietta sulla sfera;
5. Fare documentazione UML delle strutture e del coldice;
6. Creare algoritmo di stampa dei file di output;
7. Crare algoritmo che trova il cammino minimo del grafo.
8. Il codice del prof. Vicini esporta solo triangoli (pare), quindi possiamo esportare solo quelli su ParaView per provare a visualizzare i poliedri.


Documentazione con PlantUML:

Activity diagram per spiegare le condizioni in main e i poliedri che esporta
Activity diagram per le funzioni più particolari del nostro programma
Sequence diagram per spiegare il processo seguito dal programma
Class diagram per gli oggetti del nostro codice
MindMaps per esporre concetti (potrebbero essere utili)

Sequence diagram per il processo seguito del codice

@startuml
'skinparam plain true
skinparam titleBorderRoundCorner 10
skinparam titleBorderThickness 2
skinparam titleBorderColor black
skinparam titleFontSize 20
title Code sequence for a <color #76232E>geodetic polyhedron</color>
skinparam sequenceMessageAlign center
skinparam responseMessageBelowArrow true
participant main as m
collections GEOPolyhedron as GP
'box "Tessellations algorithms" #C0DFB1
participant TypeITessellation as TI
participant TypeIITessellation as TII
'end box
participant OntoTheUnitSphere as OS
'box "Minimum path \nfinding algorithms" #B5D1E8
participant BFS
participant Dijkstra as Dj
'end box

'separator for each major part of code
== Building the starting polyhedron ==

m -\ GP ** : (//p//, //q//)
m \- GP : **<color #470968>polyhedron</color> **
||15||
== Tessellating ==

'rnote over Ts #FDDE03 : Choosing the \n right tessellation

group#8EC740 Type I Tessellation [if  b > 0, c = 0 or b = 0, c > 0]
    m -\ TI ** : **<color #470968>polyhedron</color> **
    m \-- TI : **<color #FD5201>tessellated</color>** polyhedron
    end

group#06B25A Type II Tessellation [if  b > 0, c > 0 and b = c]
    m -\ TII ** : **<color #470968>polyhedron</color> **
    m \-- TII : **<color #FD5201>tessellated</color>** polyhedron
    end
||15|| 

== Projecting onto the sphere ==

m -\ OS ** : **<color #FD5201>tessellated</color>** polyhedron
m \-- OS : **<color #76232E>geodetic polyhedron</color>**
||15|| 

== Finding the minimum path ==
group#8EC740 Type I Tessellation
    m -\ BFS ** : **<color #FD5201>tessellated</color>** polyhedron and (//idVert1//, //idVert2//)
    m \-- BFS : **<color #022F73>minimum path</color>** 
    end
    
group#06B25A Type II Tessellation
    m -\ Dj ** : **<color #FD5201>tessellated</color>** polyhedron and (//idVert1//, //idVert2//)
    m \-- Dj : **<color #022F73>minimum path</color>** 
    end
    
||15||

== Exporting ==
m -> GP : **<color #76232E>geodetic polyhedron</color>** \nand **<color #022F73>minimum path</color>** 
GP ->]  ** : exporting using //UCDUtilities// on //.inp// files inside //PolygonalData//




@enduml