# ProgettoPCS2025

Code and documentation for the PCS project 2025

Domande:
1. Il file .txt può essere un .csv oppure un file che ha degli spazi come delimitatori? 

   Risposta: possiamo formattarli come vogliamo, basta che l'estensione sia .txt


2. Dobbiamo solo fare in modo che il programma dia output solo per poliedri che hanno p = 3, così come dia il poliedro di Goldberg    solo per quelli con q = 3 ?

   Risposta: conferma. Per ottenere i poliedri di Goldberg per q=3 (partendo dal cubo e dodecaedro) si passa dai poliedri con p=3 (tetraedro, ottaedro, icosaedro), quindi sono questi ultimi quelli su cui dobbiamo concentrarci. Se ci dessero come _input_ {4, 3}, possiamo partire da {3, 4} e svolgere ciò che ci è richiesto.


3. Come possiamo ottenere il _dataset_ dei vertici dei poliedri di partenza?

   Risposta: alla prof. non interessa come arriviamo ai vertici di partenza, ma consiglia di usare carta e penna, perché così i vertici a cui arriviamo sono corretti e il costo computazionale del codice diventa minore. Con il nostro algoritmo si ottiene un programma più generalizzato, quindi consiglia poi di farlo alla fine di tutto il programma.


4. Come si ottiene la tassellazione delle facce che rappresenta il primo passo per trovare il poliedro geodetico? 

   Risposta: pare che non esistano algoritmi online per spiegare i casi più semplici di tassellazione (ci sono solo per $b!=c$ che non ci interessa per il progetto). La prima tassellazione è basata sul fatto che dividiamo per b ciascun lato (in modo da ottenere una divisione equispaziata), così da ottenere i vertici esterni che poi vanno connessi tra loro. I vertici interni possono essere ottenuti come intersezioni dei lati interni alle facce così ottenuti oppure come divisione equispaziata dei segmenti paralleli ai lati (a voce ve lo potrò spiegare meglio, altrimenti chiedete pure alla Teora). 
   La seconda tasselazione, invece, si può ottenere a partire dalla prima trovando i baricentri di tutte le facce trovate con la prima tassellazione e poi unirli con i baricentri delle facce adiacenti, se esistono, oppure con i punti medi dei lati della faccia a cui appartiene il baricentro.


5. La triangolazione di tipo II non deve avere i triangoli rossi, vero? 

   Risposta: i triangoli rossi nei disegni servono solo come punto di partenza per la tassellazione di tipo II, ma non fanno parte di quest'ultima.

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
