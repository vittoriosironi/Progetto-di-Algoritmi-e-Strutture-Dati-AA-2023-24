# PSEUDOCODE

``` C
// --- DICHIARAZIONI STRUTTURE ---
// Coda
// Hash table
// Pila

// --- DICHIARAZIONE ALGORITMI ---
// Algoritmo di ordinamento


// --- LOGICA DEL CODICE ---
// --- Gestione Corriere ---
FUNZIONE-EVASIONE-ORDINI() {
    Prende i primi ordini fino a raggiungimento carico massimo
    Restituisce l array degli ordini da evadere
}

FUNZIONE-ORDINA-ORDINI-perPESO() {
    Uso un algoritmo che ordina l array per il peso
    Restituisce l array ordinato
}

FUNZIONE-PRINT-CAMIONCINO() {
    Stampa a video degli ordini caricati come: <istanti di arrivo ordine> <nome ricetta> <numero elementi ordinati>
    Se l array passato è vuoto stampo camioncino vuoto
}

FUNZIONE-CARICA-CORRIRERE() {
    Guarda nella coda degli ordini
        FUNZIONE-EVASIONE-ORDINI
    FUNZIONE-ORDINA-ORDINI-perPESO
    
    FUNZIONE-PRINT-CAMIONCINO
}

// --- Gestione aggiunta ricetta ---
FUNZIONE-PRINT-AGGIUNTA-RICETTA() {
    Se già presente la ricetta stampo ignorato
    Altrimenti stampo aggiunta
}

FUNZIONE-AGGIUNTA-RICETTA() {
    Prendo l hash table ed inserisco la ricetta
    Creo per ogni ingrediente uno spazio in memoria
    Assioco quel ingrediente alla ricetta

    FUNZIONE-PRINT-RICETTA
}

// --- Gestione rimuovi ricetta ---
FUNZIONE-PRINT-RIMOZIONE-RICETTA() {
    Se non è presente stampo non presente
    Se ci sono ordini in sospeso stampo ordini in sospeso
    Altrimenti rimossa
}

FUNZIONE-RIMOZIONE-RICETTA() {
    Prima di accedere all hash table per rimuoverla controllo se ci sono ordini da spedire
    Rimuovo la ricetta

    FUNZIONE-PRINT-RIMOZIONE-RICETTA
}

// --- Gestione rifornimento ---
FUNZIONE-PRINT-RIFORNIMENTO() {
    Stampo rifornito
}

FUNZIONE-RIFORNIMENTO() {
    Cerco ingrediente nell hash table
    Creo nuovo elemento della sottolista - lista che è in ordine di data di scadenza - quindi riordino la lista

    FUNZIONE-PRINT-RIFORNIMENTO
}

// --- Gestione ordine ---
FUNZIONE-PRINT-ORDINE() {
    Stampo accettato oppure rifiutato se non esiste nessuna ricetta con il nome specificato
}

FUNZIONE-ORDINE() {
    Controlliamo che ci sono abbastanza ingredienti per farlo
    Se si mando nella cosa degli ordini fatti
    Altrimenti mando nella coda degli ordini in attesa

    FUNZIONE-PRINT-ORDINE
}


int main() {
    Lettura configurazione del corriere

    Inizio istanti di tempo con un while
        Ogni tot di cicli faccio entrare in gioco il camioncino
            FUNZIONE-CARICA-CORRIERE

        Suddivido con uno switch comando in ingresso
            AGGIUNGI-RICETTA
                READ
            RIMUOVI-RICETTA
                READ
            RIFORNIMENTO
                READ
            ORDINE
                READ

    RICORDARSI DI FARE LE VARIE FREE
}
```