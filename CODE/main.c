#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define CONST_HASH 0.6180339887 // (sqrt(5) - 1) / 2
#define M_HASH_TABLE_RICETTE 991
#define M_HASH_TABLE_LOTTI 97
#define MAX_LENGTH 110
#define AGGIUNGI_RICETTA "aggiungi_ricetta"
#define RIMUOVI_RICETTA "rimuovi_ricetta"
#define RIFORNIMENTO "rifornimento"
#define ORDINE "ordine"

int PERIODO_CORRIERE, CAPIENZA_CORRIERE;

// STRUTTURE
// --- Ingredienti Lista ---
struct Ingrediente {
    struct Ingrediente *prev, *next;
    struct Lotto *lotto;
    int qta;
};
struct Lista_Ingredienti {
    struct Ingrediente *head;
    struct Ingrediente *tail;
};

// --- Ricetta Hash Table ---
struct Ricetta {
    struct Ricetta *prev, *next;
    char *nome;
    struct Lista_Ingredienti *ingredienti;
    int peso;
};
struct Lista_Ricette {
    struct Ricetta *head;
};

// --- Lotti Hash Table ---
struct Foglia_Lotto {
    struct Foglia_Lotto *right, *left, *padre;
    int qta;
    int scadenza;
};
struct Albero_Lotti {
    struct Foglia_Lotto *root;
    struct Foglia_Lotto *min;
};
struct Lotto {
    struct Lotto *prev, *next;
    char *nome;
    int qta_totale;
    struct Albero_Lotti *sub_lotti;
};
struct Lista_Lotti {
    struct Lotto *head;
    struct Lotto *tail;
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
    struct Ordine *min;
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
};

// --- FUNZIONI ---
unsigned int hashing(const char *str, int dim);

int lista_ricette_insert(struct Lista_Ricette *ricette, struct Ricetta* x);
int lista_ricetta_delete(struct Lista_Ricette* ricette, char nome[]);
struct Ricetta* lista_ricette_creazione_nodo(char nome[]);
struct Ricetta* lista_ricetta_search(struct Lista_Ricette* ricette, char nome[]);
void aggiunta_ricetta(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[]);
void rimozione_ricetta(struct Lista_Ricette *ricette_hashTable[], struct Lista_Ordini_Attesa *ordini_attesa_lista, struct Albero_Ordini *ordini_albero);

struct Ingrediente* lista_ingredienti_creazione_nodo(struct Lotto *lotto, int qta);
void lista_ingredienti_insert(struct Lista_Ingredienti *ingredienti, struct Ingrediente *x);
void lista_ingredienti_delete(struct Lista_Ingredienti **ingredienti);

struct Lotto* lista_lotti_insert(struct Lista_Lotti *lotti, struct Lotto* x);
struct Lotto* lista_lotti_creazione_nodo(char nome[], int qta);
struct Lotto* lista_lotti_search(struct Lista_Lotti* lista_lotto, char nome[]);
void lista_lotti_delete(struct Lista_Lotti* lotti, struct Lotto *x);
struct Foglia_Lotto* albero_lotti_creazione_foglia(int qta, int scadenza);
struct Foglia_Lotto* albero_lotti_min(struct Foglia_Lotto* root);
struct Foglia_Lotto* albero_lotti_successor(struct Foglia_Lotto* x);
void albero_lotti_delete(struct Lotto* lotto, struct Foglia_Lotto* sub_lotto, struct Foglia_Lotto* sub_lotto_successor);
void albero_lotti_insert(struct Albero_Lotti* sub_lotti, struct Foglia_Lotto* x);
void rifornimento(struct Lista_Lotti* lotti_hashTable[], struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Albero_Ordini* ordini_albero, int tempo);

struct Ordine* albero_ordini_creazione_foglia(struct Ricetta *ricetta, int n_elementi, int data);
void albero_ordini_insert(struct Albero_Ordini *ordini_albero, struct Ordine *x);
struct Ordine* albero_ordini_min(struct Ordine* root);
struct Ordine* albero_ordini_successor(struct Ordine* ordine);
void albero_ordini_delete(struct Albero_Ordini *ordini_albero, struct Ordine *ordine, struct Ordine *ordine_successor);
int albero_ordini_search(struct Ordine *ordine, char ricetta[]);
void aggiunta_ordine(struct Albero_Ordini *ordini_albero, struct Lista_Ricette *ricette_hashTable[], struct Lista_Ordini_Attesa* ordini_attesa_lista, int tempo);
int ordine_controllo_attesa(struct Ricetta* ricetta, int n_elementi, int tempo);
void ricalcolo_qta_totale(struct Lotto* lotto, int tempo);
int preparazione_ordine(struct Lista_Ordini_Attesa *ordini_attesa_lista, struct Ricetta* ricetta, int n_elementi, int tempo);
void ordine_utilizzo_ingredienti(struct Ricetta* ricetta, int n_elementi, int tempo);

void controllo_ordini_attesa(struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Albero_Ordini* ordini_albero, int tempo);
void lista_ordini_attesa_insert(struct Lista_Ordini_Attesa* lista_ordine_attesa, struct Ordine_Attesa *x);
struct Ordine_Attesa* lista_ordini_attesa_creazione_nodo(int n_elementi, int data, struct Ricetta* ricetta);
struct Ordine_Attesa* lista_ordini_attesa_search(struct Lista_Ordini_Attesa* ordine_attesa_lista, char nome[]);
struct Ordine_Attesa* lista_ordini_attesa_delete(struct Lista_Ordini_Attesa* ordini_attesa, struct Ordine_Attesa *x);

struct Ordine_Corriere* albero_ordini_corriere_creazione_foglia(struct Ricetta *ricetta, int n_elementi, int data);
void albero_ordini_corriere_insert(struct Albero_Ordini_Corriere *ordini_corriere_albero, struct Ordine_Corriere *x);
void evasione_ordini(struct Albero_Ordini* ordini);


// --- UTILITIES ---
void print_error();
void print_corriere(struct Albero_Ordini_Corriere* ordini_corriere);
void print_corriere_foglia(struct Ordine_Corriere* ordine);
char* alloca_stringa(char stringa[MAX_LENGTH]);
void dealloca_stringa(char **stringa);
int inizializzazione_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Albero_Ordini **ordini_albero, struct Lista_Ordini_Attesa **ordini_attesa_lista);
float decimal(float num);
void swap (int *x, int *y);

// --- FREE ---
void free_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Albero_Ordini **ordini_albero, struct Lista_Ordini_Attesa **ordini_attesa_lista);
void free_ricette(struct Lista_Ricette *ricette_hashTable[]);
void free_lotti(struct Lista_Lotti *lotti_hashTable[]);
void free_albero_sub_lotti(struct Foglia_Lotto *sub_lotto);
void free_ordini(struct Albero_Ordini** ordini_albero);
void free_albero_ordini(struct Ordine** ordine);
void free_ordini_attesa(struct Lista_Ordini_Attesa **ordini_attesa_lista);
void free_ordini_corriere(struct Albero_Ordini_Corriere **ordini_corriere);
void free_albero_ordini_corriere(struct Ordine_Corriere **ordine);


int main() {
    /*FILE * retIn = freopen("./../test_cases_pubblici/open10.txt", "r", stdin);
    FILE * retOut = freopen("./out.txt", "w", stdout);

    if(retIn == NULL || retOut == NULL)
        return 1;*/

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

    if(scanf("%d %d", &PERIODO_CORRIERE, &CAPIENZA_CORRIERE)) {}

    for(tempo = 0; pass != -1; tempo++) {
        pass = scanf("%s", input);

        if (tempo % PERIODO_CORRIERE == 0 && tempo != 0)
            evasione_ordini(ordini_albero);

        if (pass != -1) {
            if (!strcmp(input, AGGIUNGI_RICETTA)) {
                aggiunta_ricetta(ricette_hashTable, lotti_hashTable);
            } else if (!strcmp(input, RIMUOVI_RICETTA)) {
                rimozione_ricetta(ricette_hashTable, ordini_attesa_lista, ordini_albero);
            } else if (!strcmp(input, RIFORNIMENTO)) {
                rifornimento(lotti_hashTable, ordini_attesa_lista, ordini_albero, tempo);
            } else if (!strcmp(input, ORDINE)) {
                aggiunta_ordine(ordini_albero, ricette_hashTable, ordini_attesa_lista, tempo);
            } else { tempo--; }
        }
        memset(input, 0, sizeof(input));
    }

    free_memoria(ricette_hashTable, lotti_hashTable, &ordini_albero, &ordini_attesa_lista);

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
char* alloca_stringa(char stringa[MAX_LENGTH]) {
    char *out = malloc(strlen(stringa) * sizeof(char) + 1);
    strcpy(out, stringa);
    return out;
}
void dealloca_stringa(char **stringa) { free(*stringa); }
int inizializzazione_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Albero_Ordini **ordini_albero, struct Lista_Ordini_Attesa **ordini_attesa_lista) {
    for (int i = 0; i < M_HASH_TABLE_RICETTE; i++) {
        ricette_hashTable[i] = (struct Lista_Ricette *)calloc(1, sizeof(struct Lista_Ricette));
        if (ricette_hashTable[i] == NULL)
            return 1;
    }

    for (int i = 0; i < M_HASH_TABLE_LOTTI; i++) {
        lotti_hashTable[i] = (struct Lista_Lotti *)calloc(1, sizeof(struct Lista_Lotti));
        if (lotti_hashTable[i] == NULL)
            return 1;
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

// --- FREE ---
void free_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Albero_Ordini **ordini_albero, struct Lista_Ordini_Attesa **ordini_attesa_lista) {
    free_ricette(ricette_hashTable);
    free_lotti(lotti_hashTable);
    free_ordini(ordini_albero);
    free_ordini_attesa(ordini_attesa_lista);
}
void free_ricette(struct Lista_Ricette *ricette_hashTable[]) {
    struct Ricetta* ricetta_nodo, *ricetta_temp;

    for(int i = 0; i < M_HASH_TABLE_RICETTE; i++) {
        ricetta_nodo = ricette_hashTable[i]->head;

        while(ricetta_nodo) {
            ricetta_temp = ricetta_nodo->next;

            lista_ingredienti_delete(&(ricetta_nodo->ingredienti));

            dealloca_stringa(&(ricetta_nodo->nome));
            free(ricetta_nodo->ingredienti);
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
            dealloca_stringa(&(nodo->nome));
            if (nodo->sub_lotti)
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
void free_ordini(struct Albero_Ordini** ordini_albero) {
    free_albero_ordini(&((*ordini_albero)->root));
    free(*ordini_albero);
    *ordini_albero = NULL;
}
void free_albero_ordini(struct Ordine** ordine) {
    if (*ordine == NULL)
        return;
    free_albero_ordini(&((*ordine)->left));
    free_albero_ordini(&((*ordine)->right));
    free(*ordine);
}
void free_ordini_attesa(struct Lista_Ordini_Attesa **ordini_attesa_lista) {
    struct Ordine_Attesa *ordine_attesa = (*ordini_attesa_lista)->head;
    struct Ordine_Attesa *temp;

    while(ordine_attesa) {
        temp = ordine_attesa->next;
        free(ordine_attesa);
        ordine_attesa = temp;
    }
    free(*ordini_attesa_lista);
    *ordini_attesa_lista = NULL;
}
void free_ordini_corriere(struct Albero_Ordini_Corriere** ordini_corriere_albero) {
    free_albero_ordini_corriere(&((*ordini_corriere_albero)->root));
    free(*ordini_corriere_albero);
    *ordini_corriere_albero = NULL;
}
void free_albero_ordini_corriere(struct Ordine_Corriere** ordine) {
    if (*ordine == NULL)
        return;
    free_albero_ordini_corriere(&((*ordine)->left));
    free_albero_ordini_corriere(&((*ordine)->right));
    free(*ordine);
    ordine = NULL;
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

    x->nome = malloc(strlen(nome) * sizeof(char) + 1);
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
    } else {
        ricette->head = x;
    }

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

        if (nodo->next != NULL)
            nodo->next->prev = nodo->prev;
        
        lista_ingredienti_delete(&(nodo->ingredienti));

        free(nodo->nome);
        free(nodo);
        nodo = NULL;
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

// --- Gestione aggiunta ricetta ---
// aggiungi_ricetta <nome_ricetta> <nome_ingrediente> <quantità> ...
void aggiunta_ricetta(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[]) {
    unsigned int hash;
    int doppione = 0, qta = 0;
    char nome_ricetta[MAX_LENGTH], nome_ingrediente[MAX_LENGTH], controllo = ' ';
    struct Lista_Ricette* lista_ricetta;
    struct Lista_Ingredienti* lista_ingredienti;
    struct Lista_Lotti* lista_lotto;
    struct Lotto* lotto, *lotto_doppione;
    struct Ricetta* ricetta;
    struct Ingrediente* ingrediente;

    if (scanf("%s", nome_ricetta)) {}
    hash = hashing(nome_ricetta, M_HASH_TABLE_RICETTE);
    lista_ricetta = ricette_hashTable[hash];

    ricetta = lista_ricette_creazione_nodo(nome_ricetta);
    doppione = lista_ricette_insert(lista_ricetta, ricetta);
    if (doppione) {
        free(ricetta);
        ricetta = NULL;
        leggi_fine_riga();
        printf("ignorato\n");
        return;
    }
    
    ricetta->ingredienti = calloc(1, sizeof(struct Lista_Ingredienti));
    lista_ingredienti = ricetta->ingredienti;

    ricetta->peso = 0;
    while(controllo != '\n') {
        if (scanf("%s", nome_ingrediente)) {}
        if (scanf("%d", &qta)) {}

        hash = hashing(nome_ingrediente, M_HASH_TABLE_LOTTI);
        lista_lotto = lotti_hashTable[hash];

        lotto = lista_lotti_creazione_nodo(nome_ingrediente, 0);
        lotto_doppione = lista_lotti_insert(lista_lotto, lotto);
        if (lotto_doppione) {
            dealloca_stringa(&(lotto->nome));
            free(lotto);
            lotto = NULL;
        }
    
        ingrediente = lista_ingredienti_creazione_nodo((lotto_doppione == NULL ? lotto : lotto_doppione), qta);
        lista_ingredienti_insert(lista_ingredienti, ingrediente);
        ricetta->peso += ingrediente->qta;

        if(scanf("%c", &controllo)) {}
        memset(nome_ingrediente, 0, sizeof(nome_ingrediente));
    }

    printf("aggiunta\n");
}

// --- Gestione rimozione ricetta ---
// rimuovi_ricetta <nome_ricetta>
void rimozione_ricetta(struct Lista_Ricette *ricette_hashTable[], struct Lista_Ordini_Attesa *ordini_attesa_lista, struct Albero_Ordini *ordini_albero) {
    unsigned int hash;
    int eliminata = 0, presente;
    char nome_ricetta[MAX_LENGTH];
    struct Lista_Ricette* lista_ricetta;
    struct Ordine_Attesa* ordine_attesa;

    if (scanf("%s", nome_ricetta)) {}
    hash = hashing(nome_ricetta, M_HASH_TABLE_RICETTE);
    lista_ricetta = ricette_hashTable[hash];

    ordine_attesa = lista_ordini_attesa_search(ordini_attesa_lista, nome_ricetta);
    if (ordine_attesa) {
        printf("ordini in sospeso\n");
        return;
    }
    presente = albero_ordini_search(ordini_albero->root, nome_ricetta);
    if (presente) {
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

struct Ingrediente* lista_ingredienti_creazione_nodo(struct Lotto* lotto, int qta) {
    struct Ingrediente *x = calloc(1, sizeof(struct Ingrediente));
    if (x == NULL)
        print_error();

    x->lotto = lotto;
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
}
void lista_ingredienti_delete(struct Lista_Ingredienti **ingredienti) {
    struct Ingrediente* ingrediente = (*ingredienti)->head, *ingrediente_tmp;
    while (ingrediente)
    {
        ingrediente_tmp = ingrediente->next;
        free(ingrediente);
        ingrediente = ingrediente_tmp;
    }
    free(*ingredienti);
    *ingredienti = NULL;
}

// --- FUNZIONI LOTTI ---
// --- Lista Lotti ---
struct Lotto *lista_lotti_creazione_nodo(char nome[], int qta) {
    struct Lotto* x = calloc(1, sizeof(struct Lotto));
    if (x == NULL)
        print_error();

    x->nome = malloc(strlen(nome) * sizeof(char) + 1);
    strcpy(x->nome, nome);
    x->qta_totale = qta;

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
    
    if (x != lotti->tail)
        x->next->prev = x->prev;
    else
        lotti->tail = x->prev;
    
    if (!lotti->head)
        lotti->tail = NULL;
    free(x->nome);
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
    struct Foglia_Lotto* y = NULL;

    while(foglia) {
        y = foglia;
        if (x->scadenza < foglia->scadenza)
            foglia = foglia->left;
        else
            foglia = foglia->right;
    }
    x->padre = y;

    if (y == NULL)
        sub_lotti->root = x;
    else if (x->scadenza < y->scadenza)
        y->left = x;
    else
        y->right = x;

    if (y == NULL || sub_lotti->min->scadenza > x->scadenza)
        sub_lotti->min = x;
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
void albero_lotti_delete(struct Lotto* lotto, struct Foglia_Lotto* sub_lotto, struct Foglia_Lotto* sub_lotto_successor) {
    struct Foglia_Lotto *x, *y;

    if (sub_lotto == lotto->sub_lotti->min)
        lotto->sub_lotti->min = sub_lotto_successor;

    if (sub_lotto->left == NULL || sub_lotto->right == NULL)
        y = sub_lotto;
    else
        y = sub_lotto_successor;

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
        sub_lotto->qta = y->qta;
        sub_lotto->scadenza = y->scadenza;
    }
    free(y);
    if (lotto->sub_lotti->root == NULL) {
        free(lotto->sub_lotti);
        lotto->sub_lotti = NULL;
    }
}

// --- Gestione rifornimento ----
// rifornimento <nome_ingrediente> <qta> <scadenza> ...
void rifornimento(struct Lista_Lotti* lotti_hashTable[], struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Albero_Ordini* ordini_albero, int tempo) {
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

        if (tempo < scadenza) {
            hash = hashing(nome_ingrediente, M_HASH_TABLE_LOTTI);
            lista_lotto = lotti_hashTable[hash];

            lotto = lista_lotti_creazione_nodo(nome_ingrediente, qta);
            lotto_doppione = lista_lotti_insert(lista_lotto, lotto);
            if (lotto_doppione) {
                free(lotto);
                lotto = NULL;

                if (lotto_doppione->sub_lotti == NULL)
                    lotto_doppione->sub_lotti = calloc(1, sizeof(struct Albero_Lotti));
            } else {
                lotto->sub_lotti = calloc(1, sizeof(struct Albero_Lotti));
                if (lotto->sub_lotti == NULL)
                    print_error();
            }

            sub_lotto = albero_lotti_creazione_foglia(qta, scadenza);
            albero_lotti_insert((lotto_doppione ? lotto_doppione->sub_lotti : lotto->sub_lotti), sub_lotto);
        }

        if(scanf("%c", &controllo)) {}
        memset(nome_ingrediente, 0, sizeof(nome_ingrediente));
    }

    controllo_ordini_attesa(ordini_attesa_lista, ordini_albero, tempo);

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
    struct Ordine* y = NULL;

    while(foglia) {
        y = foglia;
        if (x->data < foglia->data)
            foglia = foglia->left;
        else
            foglia = foglia->right;
    }
    x->padre = y;

    if (y == NULL)
        ordini_albero->root = x;
    else if (x->data < y->data)
        y->left = x;
    else
        y->right = x;
    
    if (y == NULL || ordini_albero->min->data > x->data)
        ordini_albero->min = x;
}
struct Ordine* albero_ordini_min(struct Ordine* root) {
    struct Ordine* foglia = root;

    while (foglia && foglia->left)
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
void albero_ordini_delete(struct Albero_Ordini *ordini_albero, struct Ordine *ordine, struct Ordine *ordine_successor) {
    struct Ordine *x, *y;

    if (ordine == ordini_albero->min)
        ordini_albero->min = ordine_successor;

    if (ordine->left == NULL || ordine->right == NULL)
        y = ordine;
    else
        y = ordine_successor;

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
        ordine->data = y->data;
        ordine->n_elementi = y->n_elementi;
        ordine->ricetta = y->ricetta;
    }
    free(y);
}
int albero_ordini_search(struct Ordine *ordine, char nome_ricetta[]) {
    int out = 0;
    if (ordine !=  NULL) {
        out = albero_ordini_search(ordine->left, nome_ricetta);

        if (out == 0)
            out = albero_ordini_search(ordine->right, nome_ricetta);
        if (!strcmp(nome_ricetta, ordine->ricetta->nome))
            out = 1;
    }
    return out;
}

// --- Gestione Ordine ---
// ordine <nome_ricetta> <numero_elementi_ordinati>
void aggiunta_ordine(struct Albero_Ordini *ordini_albero, struct Lista_Ricette *ricette_hashTable[], struct Lista_Ordini_Attesa* ordini_attesa_lista, int tempo) {
    unsigned int hash;
    int n_elementi, preparato = 0;
    char nome_ricetta[MAX_LENGTH];
    struct Ordine* ordine;
    struct Lista_Ricette* lista_ricetta;
    struct Ricetta* ricetta;

    if (scanf("%s", nome_ricetta)) {}
    if (scanf("%d", &n_elementi)) {}

    hash = hashing(nome_ricetta, M_HASH_TABLE_RICETTE);
    lista_ricetta = ricette_hashTable[hash];
    ricetta = lista_ricetta_search(lista_ricetta, nome_ricetta);

    if (ricetta) {
        preparato = preparazione_ordine(ordini_attesa_lista, ricetta, n_elementi, tempo);

        if (preparato) {
            ordine = albero_ordini_creazione_foglia(ricetta, n_elementi, tempo);
            albero_ordini_insert(ordini_albero, ordine);
        }

        printf("accettato\n");
        return;
    }

    printf("rifiutato\n");
}


void ricalcolo_qta_totale(struct Lotto* lotto, int tempo) {
    struct Foglia_Lotto *sub_lotto = lotto->sub_lotti->min;
    struct Foglia_Lotto *sub_lotto_eliminare;

    while(sub_lotto && sub_lotto->scadenza <= tempo) {
        sub_lotto_eliminare = sub_lotto;
        lotto->qta_totale -= sub_lotto->qta;

        sub_lotto = albero_lotti_successor(sub_lotto);
        albero_lotti_delete(lotto, sub_lotto_eliminare, sub_lotto);
        if (lotto->sub_lotti == NULL)
            break;
    }

}


int ordine_controllo_attesa(struct Ricetta* ricetta, int n_elementi, int tempo) {
    int qta_necessaria;
    struct Ingrediente *ingrediente = ricetta->ingredienti->head;
    struct Lotto* lotto;

    while(ingrediente) {
        lotto = ingrediente->lotto;
        qta_necessaria = ingrediente->qta * n_elementi;

        if (lotto->sub_lotti != NULL)
            ricalcolo_qta_totale(lotto, tempo);

        if (lotto->sub_lotti == NULL || lotto->qta_totale < qta_necessaria)
            return 1;
        ingrediente = ingrediente->next;
    }

    return 0;
}

int preparazione_ordine(struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Ricetta* ricetta, int n_elementi, int tempo) {
    int attesa;
    struct Ordine_Attesa* ordine_attesa;

    attesa = ordine_controllo_attesa(ricetta, n_elementi, tempo);

    if (attesa) {
        ordine_attesa = lista_ordini_attesa_creazione_nodo(n_elementi, tempo, ricetta);
        lista_ordini_attesa_insert(ordini_attesa_lista, ordine_attesa);
        return 0;
    }

    ordine_utilizzo_ingredienti(ricetta, n_elementi, tempo);
    return 1;
}
void ordine_utilizzo_ingredienti(struct Ricetta* ricetta, int n_elementi, int tempo) {
    int qta_rimanente;
    struct Lotto* lotto;
    struct Foglia_Lotto* sub_lotto, *sub_lotto_successor;
    struct Ingrediente *ingrediente = ricetta->ingredienti->head;

    while(ingrediente) {
        lotto = ingrediente->lotto;
        sub_lotto = lotto->sub_lotti->min;
        sub_lotto_successor = NULL;
        qta_rimanente = ingrediente->qta * n_elementi;

        while(qta_rimanente > 0) {

            if (sub_lotto->qta - qta_rimanente > 0) {
                sub_lotto->qta -= qta_rimanente;
                lotto->qta_totale -= qta_rimanente;
                qta_rimanente = 0;
            } else {
                qta_rimanente -= sub_lotto->qta;
                lotto->qta_totale -= sub_lotto->qta;

                sub_lotto_successor = albero_lotti_successor(sub_lotto);

                albero_lotti_delete(lotto, sub_lotto, sub_lotto_successor);
                sub_lotto = NULL;
                
            }

            if (qta_rimanente > 0)
                sub_lotto = sub_lotto_successor;

        }

        ingrediente = ingrediente->next;
    }
}

// --- Gestione Ordini Attesa ---
void controllo_ordini_attesa(struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Albero_Ordini* ordini_albero, int tempo) {
    int attesa;
    struct Ordine_Attesa* ordine_attesa = ordini_attesa_lista->head;
    struct Ordine* ordine;

    while(ordine_attesa) {
        attesa = ordine_controllo_attesa(ordine_attesa->ricetta, ordine_attesa->n_elementi, tempo);

        if (!attesa) {
            ordine_utilizzo_ingredienti(ordine_attesa->ricetta, ordine_attesa->n_elementi, tempo);

            ordine = albero_ordini_creazione_foglia(ordine_attesa->ricetta, ordine_attesa->n_elementi, ordine_attesa->data);
            albero_ordini_insert(ordini_albero, ordine);

            ordine_attesa = lista_ordini_attesa_delete(ordini_attesa_lista, ordine_attesa);
        } else
            ordine_attesa = ordine_attesa->next;
    }
}
void lista_ordini_attesa_insert(struct Lista_Ordini_Attesa* ordine_attesa_lista, struct Ordine_Attesa *x) {
    x->prev = ordine_attesa_lista->tail;
    if (ordine_attesa_lista->tail)
        ordine_attesa_lista->tail->next = x;
    ordine_attesa_lista->tail = x;
    if (ordine_attesa_lista->head == NULL)
        ordine_attesa_lista->head = x;
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
// restituisce il nodo successivo
struct Ordine_Attesa* lista_ordini_attesa_delete(struct Lista_Ordini_Attesa* ordini_attesa, struct Ordine_Attesa *x) {
    struct Ordine_Attesa* next = x->next;
    if (x != ordini_attesa->head)
        x->prev->next = x->next;
    else
        ordini_attesa->head = x->next;
    if (x != ordini_attesa->tail)
        x->next->prev = x->prev;
    else
        ordini_attesa->tail = x->prev;
    
    if (!ordini_attesa->head)
        ordini_attesa->tail = NULL;
    free(x);
    x = NULL;

    return next;
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
    struct Ordine_Corriere* y = NULL;
    int peso_attuale, peso_y;
    int nuovo_peso = x->ricetta->peso * x->n_elementi;

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

    if (y == NULL)
        ordini_corriere_albero->root = x;
    else {
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
    }
}

// --- Gestione Evasione Ordini ---
void evasione_ordini(struct Albero_Ordini* ordini) {
    struct Albero_Ordini_Corriere* ordini_corriere;
    struct Ordine* ordine, *ordine_successor;
    struct Ordine_Corriere* ordine_corriere;
    int peso_caricato = 0;

    if (ordini->root == NULL) {
        printf("camioncino vuoto\n");
        return;
    }

    ordini_corriere = (struct Albero_Ordini_Corriere *) calloc(1, sizeof(struct Albero_Ordini_Corriere));
    if (ordini_corriere == NULL) {
        print_error();
        return;
    }

    ordine = ordini->min;
    while(ordine && ordine->ricetta && CAPIENZA_CORRIERE - (peso_caricato + ordine->ricetta->peso * ordine->n_elementi) >= 0) {
        ordine_corriere = albero_ordini_corriere_creazione_foglia(ordine->ricetta, ordine->n_elementi, ordine->data);
        albero_ordini_corriere_insert(ordini_corriere, ordine_corriere);
        peso_caricato += ordine->ricetta->peso * ordine->n_elementi;

        ordine_successor = albero_ordini_successor(ordine);
        albero_ordini_delete(ordini, ordine, ordine_successor);
        if (ordini->root == NULL)
            break;

        ordine = ordine_successor;
    }

    print_corriere(ordini_corriere);

    free_ordini_corriere(&ordini_corriere);
}
