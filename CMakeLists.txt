# Minima versione di cmake richiesta per compilare il progetto
cmake_minimum_required(VERSION 3.0.0) 

# La scritta che riguarda il progetto con il suo nome, i linguaggi utilizzati e la evrsione del progetto
project(Facsimile LANGUAGES C CXX VERSION 1.0.0)

# Creiamo un variabile di CMake Pippo con valore la lista [Bob, Ana] (CMake lavora principalmente con stringhe e liste di stringhe)
set(Pippo Bob Ana)
# oppure possiamo scrivere: set(Pippo "Bob")

# Stampo il valore associato a Pippo, a cui accedo grazie all'operatore $
message($(Pippo))

# Lavoriamo con le liste
# Aggiungiamo elementi alla lista
list(APPEND Pippo "Fabio")

message(${CMAKE_CURRENT_BINARY_DIR})
message(${CMAKE_BINARY_DIR})
# Dentro CMAKE_SOURCE_DIR c'è la cartella specificata con -S scrivendo cmake nel terminale
message(${CMAKE_SOURCE_DIR})
# Dentro CMAKE_CURRENT_SOURCE_DIR c'è la cartella in cui ci troviamo, utile per i file nelle sottocartelle
message(${CMAKE_CURRENT_SOURCE_DIR})
# Dentro PROJECT_NAME c'è "Facsimile", che abbiamo scritto sopra come argomento di Project
message(${PROJECT_NAME})
message(${CMAKE_PROJECT_NAME})

## SUBDIRECTORIES
# Questo comando va a cercare il file CMakeLists.txt nella sottocartella con il nome all'interno delle parentesi; 
# tutte le sottocartelle devono avere un CMakeLists.txt
add_subdirectory(src)
add_subdirectory(ExportParaview)

# Creiamo una lista Facsimile_sources che ha come valori la concatenazione delle due liste delle sorgenti delle sottocartelle
# Se una delle due variabile è vuiota, l'output dà zero
list(APPEND ${PROJECT_NAME}_sources 
        ${src_sources} 
        ${src_paraview_sources})

list(APPEND ${PROJECT_NAME}_headers 
        ${src_headers} 
        ${src_paraview_headers})

list(APPEND ${PROJECT_NAME}_sources 
        ${src_includes} 
        ${src_paraview_includes})

## Creiamo l'eseguibile a partire dal file main.cpp
add_executable(${CMAKE_PROJECT_NAME} main.cpp
                ${${PROJECT_NAME}_sources}
                ${${PROJECT_NAME}_headers})

# Per fornire gli includes bisogna scrivere: (PUBLIC è una keyword per fare in modo che i file da includere siano pubblcii o privati, non ci serve sapere bene cosa fa per il corso)
target_include_directories(${CMAKE_PROJECT_NAME} 
                            PUBLIC 
                            ${${PROJECT_NAME}src_includes})

# Per inserire le librerie Eigen e GoogleTest copiare da un file già fatto, cambiando il nome che può essere "Sorting2" o "PolygonalMesh" con ${PROJECT_NAME}

# Per i GoogleTest va creato un nuovo eseguibile con delle nuove liste da riga 35 in poi con ${PROJECT_NAME}_TEST al posto di ${PROJECT_NAME}