# ProgettoPCS2025

Il presente progetto è stato realizzato da Anna Dalmasso, Manuel Macis e Marco Odasso. L’obiettivo del lavoro è la generazione di un poliedro di Goldberg, opportunamente tassellato e successivamente proiettato sulla sfera unitaria. Il codice sviluppato è contenuto all'interno dell'eseguibile denominato './PlatonicSolids', il quale consente la creazione del poliedro desiderato sulla base dei parametri forniti in input.

-Parametri di Input

Il programma accetta una tupla di valori come input, secondo la seguente sintassi:

   ./PlatonicSolids p q b c

Dove:

p e q: identificano il tipo di poliedro platonico di partenza (come icosaedro, dodecaedro, ecc.), secondo le convenzioni dei simboli di Schlafli {p, q}.

b e c: definiscono i parametri di tassellazione, determinando il numero di suddivisioni delle facce e la complessità della mesh generata.

-Funzionalità Aggiuntive

Oltre ai parametri sopra indicati, il programma accetta opzionalmente anche i seguenti due identificatori:

    id_vertex1 id_vertex2

Questi parametri, se forniti, permettono di calcolare il percorso minimo tra due vertici specifici del poliedro risultante, utilizzando un algoritmo di pathfinding applicato alla struttura tridimensionale generata.

-Output del Programma

Il codice produce in output tre file con estensione .inp, che contengono la descrizione del poliedro tassellato in un formato compatibile con software di conversione e visualizzazione.

-Visualizzazione del Risultato

Per visualizzare il modello generato, è necessario seguire i seguenti passaggi:

   Caricare i file .inp ottenuti sul sito https://meshconverter.it/it, convertirli da file .ucd a file .vtu.

   Una volta ottenuto il file .vtu, è possibile caricarlo nel visualizzatore online https://kitware.github.io/glance/app/ per esplorare in modo interattivo la geometria del poliedro proiettato sulla sfera.

Per maggiori informazioni consultare la cartella Documentation con una spiegazione più dettagliata del progetto. 