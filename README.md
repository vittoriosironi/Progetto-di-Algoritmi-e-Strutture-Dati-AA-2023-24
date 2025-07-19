# 🇮🇹 Progetto PASTICCERIA - Algoritmi e Strutture Dati

_Corso di Algoritmi e Strutture Dati - Politecnico di Milano_

---

## Descrizione

Questo progetto simula la gestione di una pasticceria, ottimizzando la preparazione degli ordini, la gestione delle ricette, degli ingredienti e dei lotti. Sono state utilizzate strutture dati avanzate per garantire efficienza e scalabilità.

## Funzionalità principali

- Gestione ricette: aggiunta/rimozione ricette con ingredienti e quantità.
- Gestione ingredienti: rifornimento, controllo scadenze e aggiornamento quantità.
- Gestione ordini: accettazione, attesa, evasione tramite corriere.
- Ottimizzazione: hash table, alberi binari di ricerca, liste doppiamente collegate.
- Gestione scadenze: controllo automatico ingredienti scaduti.

## Strutture dati utilizzate

- Tabelle hash per ricette e ingredienti
- Alberi binari di ricerca per ordini e lotti
- Liste doppiamente collegate per ordini in attesa e ingredienti

## Compilazione ed esecuzione

```bash
gcc main.c -o main
./main < input.txt > out.txt
```

## File principali

- `main.c` — logica principale e strutture dati
- `Specifica.pdf` — specifica del progetto
- `Testbenches` — casi di test

## Autore

**Vittorio Sironi**  
Politecnico di Milano

**Voto conseguito: 30/30**

---

# 🇬🇧 PASTICCERIA Project - Algorithms & Data Structures

_Algorithms and Data Structures Course - Politecnico di Milano_

---

## Description

This project simulates the management of a pastry shop, optimizing order preparation, recipe, ingredient, and batch management. Advanced data structures are used to ensure efficiency and scalability.

## Main Features

- Recipe management: add/remove recipes with ingredients and quantities.
- Ingredient management: restock, expiration control, quantity updates.
- Order management: accept, queue, dispatch via courier.
- Optimization: hash tables, binary search trees, doubly linked lists.
- Expiration management: automatic control of expired ingredients.

## Data Structures Used

- Hash tables for recipes and ingredients
- Binary search trees for orders and batches
- Doubly linked lists for pending orders and ingredients

## How to Compile and Run

```bash
gcc main.c -o main
./main < input.txt > out.txt
```

## Main Files

- `main.c` — main logic and data structures
- `Specifica.pdf` — project specification
- `Testbenches` — test cases

## Author

**Vittorio Sironi**  
Politecnico di Milano

**Final grade: 30/30**