#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define CONST_HASH 0.6180339887 // (sqrt(5) - 1) / 2
#define M_HASH_TABLE_RICETTE 991
#define M_HASH_TABLE_LOTTI 97
#define MAX_LENGTH 256
#define AGGIUNGI_RICETTA "aggiungi_ricetta"
#define RIMUOVI_RICETTA "rimuovi_ricetta"
#define RIFORNIMENTO "rifornimento"
#define ORDINE "ordine"

int PERIODO_CORRIERE, CAPIENZA_CORRIERE;

// STRUTTURE
// --- Ingredienti Lista ---
struct Ingrediente {
    struct Ingrediente *prev, *next;
    char nome[MAX_LENGTH];
    int qta;
};
struct Lista_Ingredienti {
    struct Ingrediente *head;
    struct Ingrediente *tail;
    int size;
};

// --- Ricetta Hash Table ---
struct Ricetta {
    struct Ricetta *prev, *next;
    char nome[MAX_LENGTH];
    struct Lista_Ingredienti *ingredienti;
    int peso;
};
struct Lista_Ricette {
    struct Ricetta *head;
    struct Ricetta *tail;
    int size;
};

// --- Lotti Hash Table ---
struct Foglia_Lotto {
    struct Foglia_Lotto *right, *left, *padre;
    int qta;
    int scadenza;
};
struct Albero_Lotti {
    struct Foglia_Lotto *root;
    int size; // numero foglie
};
struct Lotto {
    struct Lotto *prev, *next;
    char nome[MAX_LENGTH];
    struct Albero_Lotti *sub_lotti;
};
struct Lista_Lotti {
    struct Lotto *head;
    struct Lotto *tail;
    int size;
};

// --- FUNZIONI ---
unsigned int hashing(const char *str, int dim);

int lista_ricette_insert(struct Lista_Ricette *ricette, struct Ricetta* x);
struct Ricetta* lista_ricette_creazione_nodo(char nome[]);
void aggiunta_ricetta(struct Lista_Ricette *ricette_hashTable[]);

struct Ingrediente* lista_ingredienti_creazione_nodo(char nome[], int qta);
void lista_ingredienti_insert(struct Lista_Ingredienti *ingredienti, struct Ingrediente *x);

struct Lotto* lista_lotti_insert(struct Lista_Lotti *lotti, struct Lotto* x);
struct Lotto* lista_lotti_creazione_nodo(char nome[]);
struct Foglia_Lotto* albero_lotti_creazione_foglia(int qta, int scadenza);
void albero_lotti_insert(struct Albero_Lotti* sub_lotti, struct Foglia_Lotto* x);
void rifornimento(struct Lista_Lotti *lotti_hashTable[]);


// --- UTILITIES ---
void print_error();
int inizializzazione_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[]);
void free_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[]);
float decimal(float num);


int main() {
    FILE * retIn = freopen("./Casi-Test/test.txt", "r", stdin);
    //FILE * retOut = freopen("./out.txt", "w", stdout);

    if(retIn == NULL/* || retOut == NULL*/)
        return 1;

    struct Lista_Ricette *ricette_hashTable[M_HASH_TABLE_RICETTE];
    struct Lista_Lotti *lotti_hashTable[M_HASH_TABLE_LOTTI];

    int pass = 1, tempo = 0, error = 0;
    char input[MAX_LENGTH];

    error = inizializzazione_memoria(ricette_hashTable, lotti_hashTable);
    if (error) {
        print_error();
        return 1;
    }

    scanf("%d %d", &PERIODO_CORRIERE, &CAPIENZA_CORRIERE);

    for(tempo = 0; pass; tempo++) { // -> DA SISTEMARE
        //printf("%d\n", tempo);
        if (tempo) {}
        pass = scanf("%s", input);

        if (pass) {
            // printf("%s \n", input);

            if (!strcmp(input, AGGIUNGI_RICETTA)) {
                aggiunta_ricetta(ricette_hashTable);
            } else if (!strcmp(input, RIMUOVI_RICETTA)) {

            } else if (!strcmp(input, RIFORNIMENTO)) {
                rifornimento(lotti_hashTable);
            } else if (!strcmp(input, ORDINE)) {

            } else { tempo--; }
        }
        memset(input, 0, sizeof(input));
    }

    free_memoria(ricette_hashTable, lotti_hashTable);

    return 0;
}



// --- UTILITIES ---
void print_error() { printf("ERROR\n"); }
int inizializzazione_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[]) {
    for (int i = 0; i < M_HASH_TABLE_RICETTE; i++) {
        ricette_hashTable[i] = (struct Lista_Ricette *)calloc(1, sizeof(struct Lista_Ricette));
        if (ricette_hashTable[i] == NULL)
            return 1;
        ricette_hashTable[i]->size = 0;
    }

    for (int i = 0; i < M_HASH_TABLE_LOTTI; i++) {
        lotti_hashTable[i] = (struct Lista_Lotti *)calloc(1, sizeof(struct Lista_Lotti));
        if (lotti_hashTable[i] == NULL)
            return 1;
        lotti_hashTable[i]->size = 0;
    }

    return 0;
}
void free_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[]) {
    struct Ricetta* ricetta_temp;
    struct Ingrediente* ingrediente_temp;
    //struct Lotto* lotti_temp;

    for(int i = 0; i < M_HASH_TABLE_RICETTE; i++) {
        ricetta_temp = ricette_hashTable[i]->tail;

        while(ricetta_temp && ricetta_temp->prev) {
            ingrediente_temp = ricetta_temp->ingredienti->tail;

            while(ingrediente_temp && ingrediente_temp->prev) {
                ingrediente_temp = ingrediente_temp->prev;
                free(ingrediente_temp->next);
            }

            ricetta_temp = ricetta_temp->prev;
            free(ricetta_temp->next);
        }
        free(ricetta_temp);
        free(ricette_hashTable[i]);
    }

/*
    for (int i = 0; i < M_HASH_TABLE_LOTTI; i++) {
        lotti_temp = lotti_hashTable[i]->tail;
        while(lotti_temp && lotti_temp->prev) {

        }
       
    }
*/
}
float decimal(float num) { return num - (int) num; }
void leggi_fine_riga() {
    if(scanf(" %*[^\n]")) {}
    getchar(); // Consuma il carattere di nuova linea rimasto nel buffer
}


// --- FUNZIONI STRUTTURE ---
// --- Hash Table ---
unsigned int hashing(const char *str, int dim) {
    unsigned long hash = 5381;
    int i = 0;

    while (str[i] != '\0') {
        hash = hash * 33 + str[i];
        i++;
    }

    return (unsigned int) (hash % (unsigned long) dim);
}

// FUNZIONI RICETTE
// --- Lista Ricette ---
struct Ricetta* lista_ricette_creazione_nodo(char nome[]) {
    struct Ricetta* x = calloc(1, sizeof(struct Ricetta));
    if (x == NULL)
        print_error();

    strcpy(x->nome, nome);
    return x;
}
int lista_ricette_insert(struct Lista_Ricette *ricette, struct Ricetta* x) {
    struct Ricetta* nodo = ricette->head;

    if (nodo) {
        while(nodo->next && strcmp(nodo->nome, x->nome))
            nodo = nodo->next;
        if(!strcmp(nodo->nome, x->nome))
            return 1;

        nodo->next = x;
        x->prev = nodo;
        ricette->tail = x;
    } else {
        ricette->head = x;
        ricette->tail = x;
    }
    
    ricette->size += 1;
    return 0;
}

// --- Gestione aggiunta ricetta ---
// aggiungi_ricetta <nome_ricetta> <nome_ingrediente> <quantità> ...
void aggiunta_ricetta(struct Lista_Ricette *ricette_hashTable[]) {
    unsigned int hash;
    int doppione = 0, qta = 0;
    char nome_ricetta[MAX_LENGTH], nome_ingrediente[MAX_LENGTH], controllo = ' ';
    struct Lista_Ricette* lista_ricetta;
    struct Lista_Ingredienti* lista_ingredienti;
    struct Ricetta* ricetta;
    struct Ingrediente* ingrediente;

    if (scanf("%s", nome_ricetta)) {}
    hash = hashing(nome_ricetta, M_HASH_TABLE_RICETTE);
    // printf("%d", hash);
    lista_ricetta = ricette_hashTable[hash];

    ricetta = lista_ricette_creazione_nodo(nome_ricetta);
    doppione = lista_ricette_insert(lista_ricetta, ricetta);
    if (doppione) {
        free(ricetta);
        leggi_fine_riga();
        printf("ignorato\n");
        return;
    }
    
    lista_ricetta->head->ingredienti = calloc(1, sizeof(struct Lista_Ingredienti));
    lista_ingredienti = lista_ricetta->head->ingredienti;

    ricetta->peso = 0;
    while(controllo != '\n') {
        if (scanf("%s", nome_ingrediente)) {}
        if (scanf("%d", &qta)) {}
        
        ingrediente = lista_ingredienti_creazione_nodo(nome_ingrediente, qta);
        lista_ingredienti_insert(lista_ingredienti, ingrediente);
        ricetta->peso += ingrediente->qta;

        if(scanf("%c", &controllo)) {}

        memset(nome_ingrediente, 0, sizeof(nome_ingrediente));
    }

    printf("aggiunta\n");
}

// --- FUNZIONI INGREDIENTI ---
// --- Lista Ingredienti ---
struct Ingrediente* lista_ingredienti_creazione_nodo(char nome[], int qta) {
    struct Ingrediente *x = calloc(1, sizeof(struct Ingrediente));
    if (x == NULL)
        print_error();

    strcpy(x->nome, nome);
    x->qta = qta;

    return x;
}
void lista_ingredienti_insert(struct Lista_Ingredienti *ingredienti, struct Ingrediente *x) {
    x->next = ingredienti->head;
    if (ingredienti->head)
        ingredienti->head->prev = x;
    ingredienti->head = x;
}

// --- FUNZIONI LOTTI ---
// --- Lista Lotti ---
struct Lotto *lista_lotti_creazione_nodo(char nome[]) {
    struct Lotto* x = calloc(1, sizeof(struct Lotto));
    if (x == NULL)
        print_error();

    strcpy(x->nome, nome);
    return x;

}
struct Lotto* lista_lotti_insert(struct Lista_Lotti *lotti, struct Lotto *x) {
    struct Lotto* nodo = lotti->head;

    if (nodo) {
        while(nodo->next && strcmp(nodo->nome, x->nome))
            nodo = nodo->next;
        if(!strcmp(nodo->nome, x->nome))
            return nodo;

        nodo->next = x;
        x->prev = nodo;
        lotti->tail = x;
    } else {
        lotti->head = x;
        lotti->tail = x;
    }
    
    lotti->size += 1;
    return NULL;
}

// --- ALbero Lotti ---
struct Foglia_Lotto* albero_lotti_creazione_foglia(int qta, int scadenza) {
    struct Foglia_Lotto* x = calloc(1, sizeof(struct Foglia_Lotto));

    x->qta = qta;
    x->scadenza = scadenza;

    return x;
}
void albero_lotti_insert(struct Albero_Lotti* sub_lotti, struct Foglia_Lotto* x) {
    struct Foglia_Lotto* foglia = sub_lotti->root;
    struct Foglia_Lotto* y;

    if (foglia) {
        while(foglia) {
            y = foglia;
            if (x->scadenza < foglia->scadenza)
                foglia = foglia->left;
            else
                foglia = foglia->right;
        }
        x->padre = y;

        if (x->scadenza < y->scadenza)
            y->left = x;
        else
            y->right = x;

    } else {
        sub_lotti->root = x;
    }

    sub_lotti->size += 1;
}

// 34, 33
// --- Gestione rifornimento ----
// rifornimento <nome_ingrediente> <qta> <scadenza> ...
void rifornimento(struct Lista_Lotti* lotti_hashTable[]) {
    unsigned int hash;
    int qta, scadenza;
    char nome_ingrediente[MAX_LENGTH], controllo = ' ';
    struct Lista_Lotti* lista_lotto;
    struct Lotto* lotto_doppione = NULL;
    struct Lotto* lotto;
    struct Foglia_Lotto* sub_lotto;

    while(controllo != '\n') {
        if (scanf("%s", nome_ingrediente)) {}
        hash = hashing(nome_ingrediente, M_HASH_TABLE_LOTTI);
        // printf("%d", hash);
        lista_lotto = lotti_hashTable[hash];

        lotto = lista_lotti_creazione_nodo(nome_ingrediente);
        lotto_doppione = lista_lotti_insert(lista_lotto, lotto);
        if (lotto_doppione)
            free(lotto);
        else
            lotto->sub_lotti = calloc(1, sizeof(struct Albero_Lotti));
        
        if (scanf("%d", &qta)) {}
        if (scanf("%d", &scadenza)) {}
        
        sub_lotto = albero_lotti_creazione_foglia(qta, scadenza);
        albero_lotti_insert((lotto_doppione ? lotto_doppione->sub_lotti : lotto->sub_lotti), sub_lotto);

        if(scanf("%c", &controllo)) {}

        memset(nome_ingrediente, 0, sizeof(nome_ingrediente));
    }

    printf("rifornito");
}