# 🇮🇹 Progetto PASTICCERIA - Algoritmi e Strutture Dati

_Corso di Algoritmi e Strutture Dati - Politecnico di Milano_

---

## 📖 Descrizione

Questo progetto simula la gestione di una pasticceria, ottimizzando la preparazione degli ordini, la gestione delle ricette, degli ingredienti e dei lotti. Sono state utilizzate strutture dati avanzate per garantire efficienza e scalabilità.

## 🔧 Funzionalità principali

- Gestione ricette: aggiunta/rimozione ricette con ingredienti e quantità.
- Gestione ingredienti: rifornimento, controllo scadenze e aggiornamento quantità.
- Gestione ordini: accettazione, attesa, evasione tramite corriere.
- Ottimizzazione: hash table, alberi binari di ricerca, liste doppiamente collegate.
- Gestione scadenze: controllo automatico ingredienti scaduti.

## 🗃️ Strutture dati utilizzate

- Tabelle hash per ricette e ingredienti
- Alberi binari di ricerca per ordini e lotti
- Liste doppiamente collegate per ordini in attesa e ingredienti

## 🔢 Requisiti di valutazione

Il progetto è stato valutato attraverso un testbench privato che inietta un input nel file stdin e analizza l'output prodotto su stdout. Se tale output è corretto, viene valutata la durata dell'elaborazione e l'impiego massimo di memoria.

### 📊 Risultati del testbench

| VOTO | TEMPO   | MEMORIA   |
|------|---------|-----------|
| OPEN | 45.0 s  | 150.0 MiB |
| 18   | 14.0 s  | 35.0 MiB  |
| 21   | 11.5 s  | 30.0 MiB  |
| 24   | 9.0 s   | 25.0 MiB  |
| 27   | 6.5 s   | 20.0 MiB  |
| 30   | 4.0 s   | 15.0 MiB  |
| 30L  | 1.5 s   | 14.0 MiB  |

**Nota**: Il test di OPEN è stato inserito con limiti di tempo e memoria volutamente molto larghi per dare un'idea agli studenti che non avessero passato il test del 18 di capire di quanto avessero ecceduto le richieste.

L'ultima sottoposizione di questo file ha impiegato **1.514 s** per l'elaborazione e ha utilizzato **11.5 MiB** di memoria nel suo picco.

## 🛠️ Compilazione ed esecuzione

```bash
gcc main.c -o main
./main < input.txt > out.txt
```

## 📂 File principali

- `main.c` — logica principale e strutture dati
- `Specifica.pdf` — specifica del progetto
- `Testbenches` — casi di test

## ✍️ Autore

**Vittorio Sironi**  
Politecnico di Milano

**Voto conseguito: 30/30**

---

# 🇬🇧 PASTICCERIA Project - Algorithms & Data Structures

_Algorithms and Data Structures Course - Politecnico di Milano_

---

## 📖 Description

This project simulates the management of a pastry shop, optimizing order preparation, recipe, ingredient, and batch management. Advanced data structures are used to ensure efficiency and scalability.

## 🔧 Main Features

- Recipe management: add/remove recipes with ingredients and quantities.
- Ingredient management: restock, expiration control, quantity updates.
- Order management: accept, queue, dispatch via courier.
- Optimization: hash tables, binary search trees, doubly linked lists.
- Expiration management: automatic control of expired ingredients.

## 🗃️ Data Structures Used

- Hash tables for recipes and ingredients
- Binary search trees for orders and batches
- Doubly linked lists for pending orders and ingredients

## 🔢 Evaluation Criteria

The project was evaluated through a private testbench that injects input into the stdin file and analyzes the output produced on stdout. If the output is correct, the processing time and maximum memory usage are assessed.

### 📊 Testbench Results

| GRADE | TIME    | MEMORY    |
|-------|---------|-----------|
| OPEN  | 45.0 s  | 150.0 MiB |
| 18    | 14.0 s  | 35.0 MiB  |
| 21    | 11.5 s  | 30.0 MiB  |
| 24    | 9.0 s   | 25.0 MiB  |
| 27    | 6.5 s   | 20.0 MiB  |
| 30    | 4.0 s   | 15.0 MiB  |
| 30L   | 1.5 s   | 14.0 MiB  |

**Note**: The OPEN test was included with intentionally large time and memory limits to give students who did not pass the 18 test an idea of how much they exceeded the requirements.

The last submission of this file took **1.514 s** for processing and used **11.5 MiB** of memory at its peak.

## 🛠️ How to Compile and Run

```bash
gcc main.c -o main
./main < input.txt > out.txt
```

## 📂 Main Files

- `main.c` — main logic and data structures
- `Specifica.pdf` — project specification
- `Testbenches` — test cases

## ✍️ Author

**Vittorio Sironi**  
Politecnico di Milano

**Final grade: 30/30**