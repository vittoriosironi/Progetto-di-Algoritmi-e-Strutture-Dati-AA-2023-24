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
    int qta_totale;
    struct Albero_Lotti *sub_lotti;
};
struct Lista_Lotti {
    struct Lotto *head;
    struct Lotto *tail;
    int size;
};

// --- Ordini albero ---
struct Ordine {
    struct Ordine *right, *left, *padre;
    struct Ricetta *ricetta;
    int n_elementi;
    int data;
};
struct Albero_Ordini {
    struct Ordine *root;
    int size;
};

// --- Ordini Attesa lista ---
struct Ordine_Attesa {
    struct Ordine_Attesa *prev, *next;
    struct Ricetta *ricetta;
    int n_elementi;
    int data;
};
struct Lista_Ordini_Attesa {
    struct Ordine_Attesa *head;
    struct Ordine_Attesa *tail;
    int size;
};

// --- Lista Corriere ---
struct Ordine_Corriere {
    struct Ordine_Corriere *right, *left, *padre;
    struct Ricetta *ricetta;
    int n_elementi;
    int data; 
};
struct Albero_Ordini_Corriere {
    struct Ordine_Corriere *root;
    int size;
};

// --- FUNZIONI ---
unsigned int hashing(const char *str, int dim);

int lista_ricette_insert(struct Lista_Ricette *ricette, struct Ricetta* x);
int lista_ricetta_delete(struct Lista_Ricette* ricette, char nome[]);
struct Ricetta* lista_ricette_creazione_nodo(char nome[]);
struct Ricetta* lista_ricetta_search(struct Lista_Ricette* ricette, char nome[]);
void aggiunta_ricetta(struct Lista_Ricette *ricette_hashTable[]);
void rimozione_ricetta(struct Lista_Ricette *ricette_hashTable[], struct Lista_Ordini_Attesa *ordini_attesa_lista);

struct Ingrediente* lista_ingredienti_creazione_nodo(char nome[], int qta);
void lista_ingredienti_insert(struct Lista_Ingredienti *ingredienti, struct Ingrediente *x);

struct Lotto* lista_lotti_insert(struct Lista_Lotti *lotti, struct Lotto* x);
struct Lotto* lista_lotti_creazione_nodo(char nome[], int qta);
struct Lotto* lista_lotti_search(struct Lista_Lotti* lista_lotto, char nome[]);
void lista_lotti_delete(struct Lista_Lotti* lotti, struct Lotto *x);
struct Foglia_Lotto* albero_lotti_creazione_foglia(int qta, int scadenza);
struct Foglia_Lotto* albero_lotti_min(struct Foglia_Lotto* root);
struct Foglia_Lotto* albero_lotti_successor(struct Foglia_Lotto* x);
void albero_lotti_delete(struct Lista_Lotti *lista_lotti, struct Lotto* lotto, struct Foglia_Lotto* sub_lotto);
void albero_lotti_insert(struct Albero_Lotti* sub_lotti, struct Foglia_Lotto* x);
void rifornimento(struct Lista_Lotti* lotti_hashTable[], struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Albero_Ordini* ordini_albero);

struct Ordine* albero_ordini_creazione_foglia(struct Ricetta *ricetta, int n_elementi, int data);
void albero_ordini_insert(struct Albero_Ordini *ordini_albero, struct Ordine *x);
struct Ordine* albero_ordini_min(struct Ordine* root);
struct Ordine* albero_ordini_successor(struct Ordine* ordine);
void albero_ordini_delete(struct Albero_Ordini *ordini_albero, struct Ordine *x);
void aggiunta_ordine(struct Albero_Ordini *ordini_albero, struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Lista_Ordini_Attesa* ordini_attesa_lista, int tempo);
int ordine_controllo_attesa(struct Lista_Lotti *lotti_hashTable[], struct Ricetta* ricetta, int n_elementi);
int preparazione_ordine(struct Lista_Lotti *lotti_hashTable[], struct Lista_Ordini_Attesa *ordini_attesa_lista, struct Ricetta* ricetta, int n_elementi, int data);
void ordine_utilizzo_ingredienti(struct Lista_Lotti *lotti_hashTable[], struct Ricetta* ricetta, int n_elementi);

void controllo_ordini_attesa(struct Lista_Lotti *lotti_hashTable[], struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Albero_Ordini* ordini_albero);
void lista_ordini_attesa_insert(struct Lista_Ordini_Attesa* lista_ordine_attesa, struct Ordine_Attesa *x);
struct Ordine_Attesa* lista_ordini_attesa_creazione_nodo(int n_elementi, int data, struct Ricetta* ricetta);
struct Ordine_Attesa* lista_ordini_attesa_search(struct Lista_Ordini_Attesa* ordine_attesa_lista, char nome[]);
void lista_ordini_attesa_delete(struct Lista_Ordini_Attesa* ordini_attesa, struct Ordine_Attesa *x);

struct Ordine_Corriere* albero_ordini_corriere_creazione_foglia(struct Ricetta *ricetta, int n_elementi, int data);
void albero_ordini_corriere_insert(struct Albero_Ordini_Corriere *ordini_corriere_albero, struct Ordine_Corriere *x);
void evasione_ordini(struct Albero_Ordini* ordini);


// --- UTILITIES ---
void print_error();
void print_corriere(struct Albero_Ordini_Corriere* ordini_corriere);
void print_corriere_foglia(struct Ordine_Corriere* ordine);
int inizializzazione_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Albero_Ordini **ordini_albero, struct Lista_Ordini_Attesa **ordini_attesa_lista);
float decimal(float num);
void swap (int *x, int *y);

// --- FREE ---
void free_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Albero_Ordini *ordini_albero, struct Lista_Ordini_Attesa *ordini_attesa_lista);
void free_ricette(struct Lista_Ricette *ricette_hashTable[]);
void free_lotti(struct Lista_Lotti *lotti_hashTable[]);
void free_albero_sub_lotti(struct Foglia_Lotto *sub_lotto);
void free_ordini(struct Albero_Ordini* ordini_albero);
void free_albero_ordini(struct Ordine* ordine);
void free_ordini_attesa(struct Lista_Ordini_Attesa *ordini_attesa_lista);
void free_ordini_corriere(struct Albero_Ordini_Corriere *ordini_corriere);
void free_albero_ordini_corriere(struct Ordine_Corriere* ordine);


int main() {
    // FILE * retIn = freopen("./Casi-Test/test.txt", "r", stdin);
    FILE * retIn = freopen("./../test_cases_pubblici/open4.txt", "r", stdin);
    FILE * retOut = freopen("./out.txt", "w", stdout);

    if(retIn == NULL || retOut == NULL)
        return 1;

    struct Lista_Ricette *ricette_hashTable[M_HASH_TABLE_RICETTE];
    struct Lista_Lotti *lotti_hashTable[M_HASH_TABLE_LOTTI];
    struct Albero_Ordini *ordini_albero;
    struct Lista_Ordini_Attesa *ordini_attesa_lista;

    int pass = 1, tempo = 0, error = 0;
    char input[MAX_LENGTH];

    error = inizializzazione_memoria(ricette_hashTable, lotti_hashTable, &ordini_albero, &ordini_attesa_lista);
    if (error) {
        print_error();
        return 1;
    }

    scanf("%d %d", &PERIODO_CORRIERE, &CAPIENZA_CORRIERE);

    for(tempo = 0; pass != -1; tempo++) {
        pass = scanf("%s", input);

        if (tempo % PERIODO_CORRIERE == 0 && tempo != 0)
                evasione_ordini(ordini_albero);

        if (pass != -1) {

            if (!strcmp(input, AGGIUNGI_RICETTA)) {
                aggiunta_ricetta(ricette_hashTable);
            } else if (!strcmp(input, RIMUOVI_RICETTA)) {
                rimozione_ricetta(ricette_hashTable, ordini_attesa_lista);
            } else if (!strcmp(input, RIFORNIMENTO)) {
                rifornimento(lotti_hashTable, ordini_attesa_lista, ordini_albero);
            } else if (!strcmp(input, ORDINE)) {
                aggiunta_ordine(ordini_albero, ricette_hashTable, lotti_hashTable, ordini_attesa_lista, tempo);
            } else { tempo--; }
        }
        memset(input, 0, sizeof(input));
    }

    free_memoria(ricette_hashTable, lotti_hashTable, ordini_albero, ordini_attesa_lista);

    return 0;
}



// --- UTILITIES ---
void print_error() { printf("ERROR\n"); }
void print_corriere(struct Albero_Ordini_Corriere* ordini_corriere) {
    return print_corriere_foglia(ordini_corriere->root);
}
void print_corriere_foglia(struct Ordine_Corriere* ordine) {
    if (ordine == NULL)
        return;
    print_corriere_foglia(ordine->right);
    printf("%d %s %d\n", ordine->data, ordine->ricetta->nome, ordine->n_elementi);
    print_corriere_foglia(ordine->left);
}
int inizializzazione_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Albero_Ordini **ordini_albero, struct Lista_Ordini_Attesa **ordini_attesa_lista) {
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

    *ordini_albero = (struct Albero_Ordini *) calloc(1, sizeof(struct Albero_Ordini));
    if (*ordini_albero == NULL)
        return 1;
    
    *ordini_attesa_lista = (struct Lista_Ordini_Attesa *) calloc(1, sizeof(struct Lista_Ordini_Attesa));
    if (*ordini_attesa_lista == NULL)
        return 1;

    return 0;
}
float decimal(float num) { return num - (int) num; }
void leggi_fine_riga() {
    if(scanf(" %*[^\n]")) {}
    getchar(); // Consuma il carattere di nuova linea rimasto nel buffer
}
void swap (int *x, int *y) {
    int *supp;

    supp=x;
    x=y;
    y=supp;
}

// --- FREE ---
void free_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Albero_Ordini *ordini_albero, struct Lista_Ordini_Attesa *ordini_attesa_lista) {
    free_ricette(ricette_hashTable);
    free_lotti(lotti_hashTable);
    free_ordini(ordini_albero);
    free_ordini_attesa(ordini_attesa_lista);
}
void free_ricette(struct Lista_Ricette *ricette_hashTable[]) {
    struct Ricetta* ricetta_nodo, *ricetta_temp;
    struct Ingrediente* ingrediente_nodo, *ingrediente_temp;

    for(int i = 0; i < M_HASH_TABLE_RICETTE; i++) {
        ricetta_nodo = ricette_hashTable[i]->head;

        while(ricetta_nodo) {
            ricetta_temp = ricetta_nodo->next;
            ingrediente_nodo = ricetta_nodo->ingredienti->head;

            while(ingrediente_nodo) {
                ingrediente_temp = ingrediente_nodo->next;
                free(ingrediente_nodo);
                ingrediente_nodo = ingrediente_temp;
            }

            free(ricetta_nodo);
            ricetta_nodo = ricetta_temp;
        }
        free(ricette_hashTable[i]);
    }
}
void free_lotti(struct Lista_Lotti *lotti_hashTable[]) {
    struct Lotto *nodo, *temp;

    for (int i = 0; i < M_HASH_TABLE_LOTTI; i++) {
        nodo = lotti_hashTable[i]->head;
        
        while(nodo) {
            temp = nodo->next;
            free_albero_sub_lotti(nodo->sub_lotti->root);
            free(nodo);
            nodo = temp;
        }
        free(lotti_hashTable[i]);
    }
}
void free_albero_sub_lotti(struct Foglia_Lotto *sub_lotto) {
    if (sub_lotto == NULL)
        return;
    free_albero_sub_lotti(sub_lotto->left);
    free_albero_sub_lotti(sub_lotto->right);
    free(sub_lotto);
}
void free_ordini(struct Albero_Ordini* ordini_albero) {
    free_albero_ordini(ordini_albero->root);
}
void free_albero_ordini(struct Ordine* ordine) {
    if (ordine == NULL)
        return;
    free_albero_ordini(ordine->left);
    free_albero_ordini(ordine->right);
    free(ordine);
}
void free_ordini_attesa(struct Lista_Ordini_Attesa *ordini_attesa_lista) {
    struct Ordine_Attesa *ordine_attesa = ordini_attesa_lista->head;
    struct Ordine_Attesa *temp;

    while(ordine_attesa) {
        temp = ordine_attesa->next;
        free(ordine_attesa);
        ordine_attesa = temp;
    }
}
void free_ordini_corriere(struct Albero_Ordini_Corriere* ordini_corriere_albero) {
    free_albero_ordini_corriere(ordini_corriere_albero->root);
}
void free_albero_ordini_corriere(struct Ordine_Corriere* ordine) {
    if (ordine == NULL)
        return;
    free_albero_ordini_corriere(ordine->left);
    free_albero_ordini_corriere(ordine->right);
    free(ordine);
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
int lista_ricetta_delete(struct Lista_Ricette* ricette, char nome[]) {
    struct Ricetta* nodo = ricette->head;

    nodo = lista_ricetta_search(ricette, nome);

    if(nodo && !strcmp(nodo->nome, nome)) {
        if (nodo != ricette->head)
            nodo->prev->next = nodo->next;
        else
            ricette->head = nodo->next;

        if (nodo->next)
            nodo->next->prev = nodo->prev;
        
        ricette->size -= 1;

        if (!ricette->head)
            ricette->tail = NULL;

        free(nodo);
        return 1;
    }

    return 0;
}
struct Ricetta* lista_ricetta_search(struct Lista_Ricette* ricette, char nome[]) {
    struct Ricetta* nodo = ricette->head;

    while(nodo && strcmp(nodo->nome, nome))
        nodo = nodo->next;
    
    return nodo;
}

// 504 - 30
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

// 729
// --- Gestione rimozione ricetta ---
// rimuovi_ricetta <nome_ricetta>
void rimozione_ricetta(struct Lista_Ricette *ricette_hashTable[], struct Lista_Ordini_Attesa *ordini_attesa_lista) {
    unsigned int hash;
    int eliminata = 0;
    char nome_ricetta[MAX_LENGTH];
    struct Lista_Ricette* lista_ricetta;
    struct Ordine_Attesa* ordine_attesa;

    if (scanf("%s", nome_ricetta)) {}
    hash = hashing(nome_ricetta, M_HASH_TABLE_RICETTE);
    // printf("%d", hash);
    lista_ricetta = ricette_hashTable[hash];

    ordine_attesa = lista_ordini_attesa_search(ordini_attesa_lista, nome_ricetta);
    if (ordine_attesa) {
        printf("ordini in sospeso\n");
        return;
    }
    eliminata = lista_ricetta_delete(lista_ricetta, nome_ricetta);

    if (eliminata)
        printf("rimossa\n");
    else
        printf("non presente\n");
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
    if (ingredienti->tail == NULL)
        ingredienti->tail = x;
    ingredienti->size += 1;
}

// --- FUNZIONI LOTTI ---
// --- Lista Lotti ---
struct Lotto *lista_lotti_creazione_nodo(char nome[], int qta) {
    struct Lotto* x = calloc(1, sizeof(struct Lotto));
    if (x == NULL)
        print_error();

    x->qta_totale = qta;
    strcpy(x->nome, nome);
    return x;
}
struct Lotto* lista_lotti_insert(struct Lista_Lotti *lotti, struct Lotto *x) {
    struct Lotto* nodo = lotti->head;

    if (nodo) {
        while(nodo->next && strcmp(nodo->nome, x->nome))
            nodo = nodo->next;
        if(!strcmp(nodo->nome, x->nome)) {
            nodo->qta_totale += x->qta_totale;
            return nodo;
        }

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
struct Lotto* lista_lotti_search(struct Lista_Lotti* lotti, char nome[]) {
    struct Lotto* nodo = lotti->head;

    while(nodo && strcmp(nodo->nome, nome))
        nodo = nodo->next;
    
    return nodo;
}
void lista_lotti_delete(struct Lista_Lotti* lotti, struct Lotto *x) {
    if (x != lotti->head)
        x->prev->next = x->next;
    else
        lotti->head = x->next;
    if (x->next)
        x->next->prev = x->prev;
    
    if (!lotti->head)
        lotti->tail = NULL;
    lotti->size -=1;
    free(x);
}

// --- Albero Lotti ---
struct Foglia_Lotto* albero_lotti_creazione_foglia(int qta, int scadenza) {
    struct Foglia_Lotto* x = calloc(1, sizeof(struct Foglia_Lotto));
    if (x == NULL)
        print_error();

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
struct Foglia_Lotto* albero_lotti_min(struct Foglia_Lotto* root) {
    struct Foglia_Lotto* foglia = root;

    while (foglia->left)
        foglia = foglia->left;
    
    return foglia;
}
struct Foglia_Lotto* albero_lotti_successor(struct Foglia_Lotto* x) {
    struct Foglia_Lotto* y;

    if (x->right != NULL)
        return albero_lotti_min(x->right);
    y = x->padre;
    
    while(y != NULL && x == y->right) {
        x = y;
        y = y->padre;
    }

    return y;
}
void albero_lotti_delete(struct Lista_Lotti *lista_lotti, struct Lotto* lotto, struct Foglia_Lotto* sub_lotto) {
    struct Foglia_Lotto *x, *y;

    if (sub_lotto->left == NULL || sub_lotto->right == NULL)
        y = sub_lotto;
    else
        y = albero_lotti_successor(sub_lotto);
    
    if (y->left != NULL)
        x = y->left;
    else
        x = y->right;
    
    if (x != NULL)
        x->padre = y->padre;
    if (y->padre == NULL)
        lotto->sub_lotti->root = x;
    else if (y == y->padre->left)
        y->padre->left = x;
    else
        y->padre->right = x;
    
    if (y != sub_lotto) {
        swap(&sub_lotto->qta, &y->qta);
        swap(&sub_lotto->scadenza, &y->scadenza);
    }

    free(sub_lotto);
    lotto->sub_lotti->size -= 1;
    if (lotto->sub_lotti->size == 0) {
        free(lotto->sub_lotti);
        lista_lotti_delete(lista_lotti, lotto);
    }
}


// --- Gestione rifornimento ----
// rifornimento <nome_ingrediente> <qta> <scadenza> ...
void rifornimento(struct Lista_Lotti* lotti_hashTable[], struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Albero_Ordini* ordini_albero) {
    unsigned int hash;
    int qta, scadenza;
    char nome_ingrediente[MAX_LENGTH], controllo = ' ';
    struct Lista_Lotti* lista_lotto;
    struct Lotto* lotto_doppione = NULL;
    struct Lotto* lotto;
    struct Foglia_Lotto* sub_lotto;

    while(controllo != '\n') {
        if (scanf("%s", nome_ingrediente)) {}
        if (scanf("%d", &qta)) {}
        if (scanf("%d", &scadenza)) {}

        hash = hashing(nome_ingrediente, M_HASH_TABLE_LOTTI);
        // printf("%d", hash);
        lista_lotto = lotti_hashTable[hash];

        lotto = lista_lotti_creazione_nodo(nome_ingrediente, qta);
        lotto_doppione = lista_lotti_insert(lista_lotto, lotto);
        if (lotto_doppione)
            free(lotto);
        else {
            lotto->sub_lotti = calloc(1, sizeof(struct Albero_Lotti));
            if (lotto->sub_lotti == NULL)
                print_error();
        }

        sub_lotto = albero_lotti_creazione_foglia(qta, scadenza);
        albero_lotti_insert((lotto_doppione ? lotto_doppione->sub_lotti : lotto->sub_lotti), sub_lotto);

        controllo_ordini_attesa(lotti_hashTable, ordini_attesa_lista, ordini_albero);

        if(scanf("%c", &controllo)) {}

        memset(nome_ingrediente, 0, sizeof(nome_ingrediente));
    }

    printf("rifornito\n");
}


// FUNZIONE ORDINI
// --- Albero Ordini ---
struct Ordine* albero_ordini_creazione_foglia(struct Ricetta *ricetta, int n_elementi, int data) {
    struct Ordine* x = calloc(1, sizeof(struct Ordine));
    if (x == NULL)
        print_error();
    
    x->data = data;
    x->n_elementi = n_elementi;
    x->ricetta = ricetta;

    return x;
}
void albero_ordini_insert(struct Albero_Ordini *ordini_albero, struct Ordine *x) {
    struct Ordine* foglia = ordini_albero->root;
    struct Ordine* y;

    if (foglia) {
        while(foglia) {
            y = foglia;
            if (x->data < foglia->data)
                foglia = foglia->left;
            else
                foglia = foglia->right;
        }
        x->padre = y;

        if (x->data < y->data)
            y->left = x;
        else
            y->right = x;

    } else {
        ordini_albero->root = x;
    }

    ordini_albero->size += 1;
}
struct Ordine* albero_ordini_min(struct Ordine* root) {
    struct Ordine* foglia = root;

    while (foglia->left)
        foglia = foglia->left;

    return foglia;
}
struct Ordine* albero_ordini_successor(struct Ordine* x) {
    struct Ordine* y;

    if (x->right != NULL)
        return albero_ordini_min(x->right);
    y = x->padre;
    
    while(y != NULL && x == y->right) {
        x = y;
        y = y->padre;
    }

    return y;
}
void albero_ordini_delete(struct Albero_Ordini *ordini_albero, struct Ordine *ordine) {
    struct Ordine *x, *y;

    if (ordine->left == NULL || ordine->right == NULL)
        y = ordine;
    else
        y = albero_ordini_successor(ordine);
    
    if (y->left != NULL)
        x = y->left;
    else
        x = y->right;
    
    if (x != NULL)
        x->padre = y->padre;
    if (y->padre == NULL)
        ordini_albero->root = x;
    else if (y == y->padre->left)
        y->padre->left = x;
    else
        y->padre->right = x;
    
    if (y != ordine) {
        swap(&ordine->data, &y->data);
        swap(&ordine->n_elementi, &y->n_elementi);
        y->ricetta = ordine->ricetta;
    }

    ordini_albero->size -= 1;
    free(ordine);
}

// --- Gestione Ordine ---
// ordine <nome_ricetta> <numero_elementi_ordinati>
void aggiunta_ordine(struct Albero_Ordini *ordini_albero, struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Lista_Ordini_Attesa* ordini_attesa_lista, int tempo) {
    unsigned int hash;
    int n_elementi, preparato = 0;
    char nome_ricetta[MAX_LENGTH];
    struct Ordine* ordine;
    struct Lista_Ricette* lista_ricetta;
    struct Ricetta* ricetta;

    if (scanf("%s", nome_ricetta)) {}
    if (scanf("%d", &n_elementi)) {}

    hash = hashing(nome_ricetta, M_HASH_TABLE_RICETTE);
    // printf("%d", hash);
    lista_ricetta = ricette_hashTable[hash];
    ricetta = lista_ricetta_search(lista_ricetta, nome_ricetta);

    if (ricetta) {
        preparato = preparazione_ordine(lotti_hashTable, ordini_attesa_lista, ricetta, n_elementi, tempo);

        if (preparato) {
            ordine = albero_ordini_creazione_foglia(ricetta, n_elementi, tempo);
            albero_ordini_insert(ordini_albero, ordine);
        }

        printf("accettato\n");
        return;
    }

    printf("rifiutato\n");
}
// 33
int ordine_controllo_attesa(struct Lista_Lotti *lotti_hashTable[], struct Ricetta* ricetta, int n_elementi) {
    unsigned int hash;
    int qta_necessaria;
    struct Ingrediente *ingrediente = ricetta->ingredienti->head;
    struct Lista_Lotti* lista_lotto;
    struct Lotto* lotto;

    while(ingrediente) {
        lotto = NULL;
        qta_necessaria = ingrediente->qta * n_elementi;

        hash = hashing(ingrediente->nome, M_HASH_TABLE_LOTTI);
        lista_lotto = lotti_hashTable[hash];
        lotto = lista_lotti_search(lista_lotto, ingrediente->nome);

        if (!lotto || lotto->qta_totale < qta_necessaria)
            return 1;

        ingrediente = ingrediente->next;
    }

    return 0;
}

int preparazione_ordine(struct Lista_Lotti *lotti_hashTable[], struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Ricetta* ricetta, int n_elementi, int data) {
    int attesa;
    struct Ordine_Attesa* ordine_attesa;

    attesa = ordine_controllo_attesa(lotti_hashTable, ricetta, n_elementi);

    if (attesa) {
        ordine_attesa = lista_ordini_attesa_creazione_nodo(n_elementi, data, ricetta);
        lista_ordini_attesa_insert(ordini_attesa_lista, ordine_attesa);
        return 0;
    }

    ordine_utilizzo_ingredienti(lotti_hashTable, ricetta, n_elementi);

    return 1;
}
void ordine_utilizzo_ingredienti(struct Lista_Lotti *lotti_hashTable[], struct Ricetta* ricetta, int n_elementi) {
    unsigned int hash;
    int qta_rimanente;
    struct Lista_Lotti* lista_lotto;
    struct Lotto* lotto;
    struct Foglia_Lotto* sub_lotto;
    struct Ingrediente *ingrediente = ricetta->ingredienti->head;

    while(ingrediente) {
        lotto = NULL;
        qta_rimanente = ingrediente->qta * n_elementi;

        hash = hashing(ingrediente->nome, M_HASH_TABLE_LOTTI);
        lista_lotto = lotti_hashTable[hash];
        lotto = lista_lotti_search(lista_lotto, ingrediente->nome);

        while(lotto && lotto->sub_lotti && qta_rimanente > 0) {
            sub_lotto = albero_lotti_min(lotto->sub_lotti->root);

            if (sub_lotto->qta - qta_rimanente > 0) {
                sub_lotto->qta -= qta_rimanente;
                qta_rimanente = 0;
            } else {
                qta_rimanente -= sub_lotto->qta;
                albero_lotti_delete(lista_lotto, lotto, sub_lotto);
                break;
            }
        }

        ingrediente = ingrediente->next;
    }
}

// --- Gestione Ordini Attesa ---
void controllo_ordini_attesa(struct Lista_Lotti *lotti_hashTable[], struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Albero_Ordini* ordini_albero) {
    int attesa;
    struct Ordine_Attesa* ordine_attesa = ordini_attesa_lista->head;
    struct Ordine* ordine;
    
    while(ordine_attesa) {
        attesa = ordine_controllo_attesa(lotti_hashTable, ordine_attesa->ricetta, ordine_attesa->n_elementi);

        if (!attesa) {
            ordine_utilizzo_ingredienti(lotti_hashTable, ordine_attesa->ricetta, ordine_attesa->n_elementi);

            ordine = albero_ordini_creazione_foglia(ordine_attesa->ricetta, ordine_attesa->n_elementi, ordine_attesa->data);
            albero_ordini_insert(ordini_albero, ordine);

            lista_ordini_attesa_delete(ordini_attesa_lista, ordine_attesa);
        }
        ordine_attesa = ordine_attesa->next;
    }
}
void lista_ordini_attesa_insert(struct Lista_Ordini_Attesa* ordine_attesa_lista, struct Ordine_Attesa *x) {
    x->next = ordine_attesa_lista->head;
    if (ordine_attesa_lista->head)
        ordine_attesa_lista->head->prev = x;
    ordine_attesa_lista->head = x;
    if (ordine_attesa_lista->tail == NULL)
        ordine_attesa_lista->tail = x;
    ordine_attesa_lista->size += 1;
}
struct Ordine_Attesa* lista_ordini_attesa_creazione_nodo(int n_elementi, int data, struct Ricetta* ricetta) {
    struct Ordine_Attesa *x = calloc(1, sizeof(struct Ordine_Attesa));
    if (x == NULL)
        print_error();

    x->data = data;
    x->n_elementi = n_elementi;
    x->ricetta = ricetta;

    return x;
}
struct Ordine_Attesa* lista_ordini_attesa_search(struct Lista_Ordini_Attesa* ordine_attesa_lista, char nome[]) {
    struct Ordine_Attesa* nodo = ordine_attesa_lista->head;

    while(nodo && strcmp(nodo->ricetta->nome, nome))
        nodo = nodo->next;
    
    return nodo;
}
void lista_ordini_attesa_delete(struct Lista_Ordini_Attesa* ordini_attesa, struct Ordine_Attesa *x) {
    if (x != ordini_attesa->head)
        x->prev->next = x->next;
    else
        ordini_attesa->head = x->next;
    if (x->next)
        x->next->prev = x->prev;
    
    if (!ordini_attesa->head)
        ordini_attesa->tail = NULL;
    ordini_attesa->size -=1;
    free(x);
}

// EVASIONE ORDINI
// --- Albero Evasione Ordini ---
struct Ordine_Corriere* albero_ordini_corriere_creazione_foglia(struct Ricetta *ricetta, int n_elementi, int data) {
    struct Ordine_Corriere* x = calloc(1, sizeof(struct Ordine_Corriere));
    if (x == NULL)
        print_error();
    
    x->data = data;
    x->n_elementi = n_elementi;
    x->ricetta = ricetta;

    return x;
}
void albero_ordini_corriere_insert(struct Albero_Ordini_Corriere *ordini_corriere_albero, struct Ordine_Corriere *x) {
    struct Ordine_Corriere* foglia = ordini_corriere_albero->root;
    struct Ordine_Corriere* y;
    int peso_attuale, peso_y;
    int nuovo_peso = x->ricetta->peso * x->n_elementi;

    if (foglia) {
        while(foglia) {
            peso_attuale = foglia->ricetta->peso * foglia->n_elementi;
            y = foglia;

            if (nuovo_peso < peso_attuale)
                foglia = foglia->left;
            else if (nuovo_peso > peso_attuale)
                foglia = foglia->right;
            else {
                if (x->data > foglia->data)
                    foglia = foglia->left;
                else
                    foglia = foglia->right;
            }
        }
        x->padre = y;

        nuovo_peso = x->ricetta->peso * x->n_elementi;
        peso_y = y->ricetta->peso * y->n_elementi;

        if (nuovo_peso < peso_y)
            y->left = x;
        else if (nuovo_peso > peso_y)
            y->right = x;
        else {
            if (x->data > y->data)
                y->left = x;
            else
                y->right = x;
        }
    } else {
        ordini_corriere_albero->root = x;
    }

    ordini_corriere_albero->size += 1;
}

// --- Gestione Evasione Ordini ---
void evasione_ordini(struct Albero_Ordini* ordini) {
    struct Albero_Ordini_Corriere* ordini_corriere;
    struct Ordine* ordine;
    struct Ordine_Corriere* ordine_corriere;
    int peso_caricato = 0;

    if (ordini->size == 0) {
        printf("camioncino vuoto\n");
        return;
    }

    ordini_corriere = (struct Albero_Ordini_Corriere *) calloc(1, sizeof(struct Albero_Ordini_Corriere));
    if (ordini_corriere == NULL) {
        print_error();
        return;
    }


    ordine = albero_ordini_min(ordini->root);
    do {
        if (ordine && ordine->ricetta && CAPIENZA_CORRIERE - (peso_caricato + ordine->ricetta->peso) >= 0) {
            ordine_corriere = albero_ordini_corriere_creazione_foglia(ordine->ricetta, ordine->n_elementi, ordine->data);
            albero_ordini_corriere_insert(ordini_corriere, ordine_corriere);
            peso_caricato += ordine->ricetta->peso * ordine->n_elementi;

            albero_ordini_delete(ordini, ordine);
        }

        if (ordini->root)
            ordine = albero_ordini_min(ordini->root);
    } while(ordine && ordine->ricetta && CAPIENZA_CORRIERE - (peso_caricato + ordine->ricetta->peso) >= 0);

    print_corriere(ordini_corriere);

    free_ordini_corriere(ordini_corriere);
}