#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define CONST_HASH 0.6180339887 // (sqrt(5) - 1) / 2
#define M_HASH_TABLE_RICETTE 991
#define M_HASH_TABLE_LOTTI 97
#define MAX_LENGTH 80
#define AGGIUNGI_RICETTA "aggiungi_ricetta"
#define RIMUOVI_RICETTA "rimuovi_ricetta"
#define RIFORNIMENTO "rifornimento"
#define ORDINE "ordine"

int PERIODO_CORRIERE, CAPIENZA_CORRIERE;

// STRUTTURE
// --- Ingredienti Lista ---
struct Ingrediente {
    struct Ingrediente *next;
    struct Lotto *lotto;
    int qta;
};
struct Lista_Ingredienti {
    struct Ingrediente *head;
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
    struct Lotto *next;
    char *nome;
    int qta_totale;
    struct Albero_Lotti *sub_lotti;
};
struct Lista_Lotti {
    struct Lotto *head;
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
    struct Ingrediente *primo_ingrediente_mancante;
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

void lista_ricette_insert(struct Lista_Ricette *ricette, struct Ricetta* x);
int lista_ricetta_delete(struct Lista_Ricette* ricette, char nome[]);
struct Ricetta* lista_ricette_creazione_nodo(char nome[]);
struct Ricetta* lista_ricetta_search(struct Lista_Ricette* ricette, char nome[]);
void aggiunta_ricetta(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[]);
void rimozione_ricetta(struct Lista_Ricette *ricette_hashTable[], struct Lista_Ordini_Attesa *ordini_attesa_lista, struct Albero_Ordini *ordini_albero);

struct Ingrediente* lista_ingredienti_creazione_nodo(struct Lotto *lotto, int qta);
void lista_ingredienti_insert(struct Lista_Ingredienti *ingredienti, struct Ingrediente *x);
void lista_ingredienti_delete(struct Lista_Ingredienti **ingredienti);

void lista_lotti_insert(struct Lista_Lotti *lotti, struct Lotto* x);
struct Lotto* lista_lotti_creazione_nodo(char nome[], int qta);
struct Lotto* lista_lotti_search(struct Lista_Lotti* lista_lotto, char nome[]);
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
struct Ingrediente* ordine_controllo_attesa(struct Ordine_Attesa* ordine_attesa, struct Ricetta* ricetta, int n_elementi, int tempo);
void ricalcolo_qta_totale(struct Lotto* lotto, int tempo);
void controllo_ordini_scaduti(struct Lista_Lotti* lotti_hashTable[], int tempo);
int preparazione_ordine(struct Lista_Ordini_Attesa *ordini_attesa_lista, struct Ricetta* ricetta, int n_elementi, int tempo);
void ordine_utilizzo_ingredienti(struct Ricetta* ricetta, int n_elementi, int tempo);

void controllo_ordini_attesa(struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Albero_Ordini* ordini_albero, int tempo);
void lista_ordini_attesa_insert(struct Lista_Ordini_Attesa* lista_ordine_attesa, struct Ordine_Attesa *x);
struct Ordine_Attesa* lista_ordini_attesa_creazione_nodo(int n_elementi, int data, struct Ricetta* ricetta, struct Ingrediente* ingrediente_mancante);
struct Ordine_Attesa* lista_ordini_attesa_search(struct Lista_Ordini_Attesa* ordine_attesa_lista, char nome[]);
struct Ordine_Attesa* lista_ordini_attesa_delete(struct Lista_Ordini_Attesa* ordini_attesa, struct Ordine_Attesa *x);

struct Ordine_Corriere* albero_ordini_corriere_creazione_foglia(struct Ricetta *ricetta, int n_elementi, int data);
void albero_ordini_corriere_insert(struct Albero_Ordini_Corriere *ordini_corriere_albero, struct Ordine_Corriere *x);
void evasione_ordini(struct Albero_Ordini* ordini);


// --- UTILITIES ---
void print_corriere(struct Albero_Ordini_Corriere* ordini_corriere);
void print_corriere_foglia(struct Ordine_Corriere* ordine);
void dealloca_stringa(char **stringa);
void inizializzazione_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Albero_Ordini **ordini_albero, struct Lista_Ordini_Attesa **ordini_attesa_lista);
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

    struct Lista_Ricette *ricette_hashTable[M_HASH_TABLE_RICETTE];
    struct Lista_Lotti *lotti_hashTable[M_HASH_TABLE_LOTTI];
    struct Albero_Ordini *ordini_albero;
    struct Lista_Ordini_Attesa *ordini_attesa_lista;

    int pass = 1, tempo = 0;
    char input[MAX_LENGTH];

    inizializzazione_memoria(ricette_hashTable, lotti_hashTable, &ordini_albero, &ordini_attesa_lista);

    if(scanf("%d %d", &PERIODO_CORRIERE, &CAPIENZA_CORRIERE)) {}

    for(tempo = 0; pass != -1; tempo++) {
        pass = scanf("%s", input);
        if (tempo != 0)
            controllo_ordini_scaduti(lotti_hashTable, tempo);
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

    //free_memoria(ricette_hashTable, lotti_hashTable, &ordini_albero, &ordini_attesa_lista);

    return 0;
}



// --- UTILITIES ---
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
void dealloca_stringa(char **stringa) { free(*stringa); }
void inizializzazione_memoria(struct Lista_Ricette *ricette_hashTable[], struct Lista_Lotti *lotti_hashTable[], struct Albero_Ordini **ordini_albero, struct Lista_Ordini_Attesa **ordini_attesa_lista) {
    for (int i = 0; i < M_HASH_TABLE_RICETTE; i++) {
        ricette_hashTable[i] = (struct Lista_Ricette *)calloc(1, sizeof(struct Lista_Ricette));
        if (ricette_hashTable[i] == NULL)
            return;
    }

    for (int i = 0; i < M_HASH_TABLE_LOTTI; i++) {
        lotti_hashTable[i] = (struct Lista_Lotti *)calloc(1, sizeof(struct Lista_Lotti));
        if (lotti_hashTable[i] == NULL)
            return;
    }

    *ordini_albero = (struct Albero_Ordini *) calloc(1, sizeof(struct Albero_Ordini));
    if (*ordini_albero == NULL)
        return;
    
    *ordini_attesa_lista = (struct Lista_Ordini_Attesa *) calloc(1, sizeof(struct Lista_Ordini_Attesa));
    if (*ordini_attesa_lista == NULL)
        return;

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

    x->nome = malloc(strlen(nome) * sizeof(char) + 1);
    strcpy(x->nome, nome);
    return x;
}

void lista_ricette_insert(struct Lista_Ricette *ricette, struct Ricetta* x) {
    x->next = ricette->head;
    if (ricette->head)
        ricette->head->prev = x;
    ricette->head = x;
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
    struct Ricetta* doppione = NULL;
    int qta = 0;
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

    doppione = lista_ricetta_search(lista_ricetta, nome_ricetta);
    if (doppione) {
        leggi_fine_riga();
        printf("ignorato\n");
        return;
    }
    ricetta = lista_ricette_creazione_nodo(nome_ricetta);
    lista_ricette_insert(lista_ricetta, ricetta);

    ricetta->ingredienti = calloc(1, sizeof(struct Lista_Ingredienti));
    lista_ingredienti = ricetta->ingredienti;

    ricetta->peso = 0;
    while(controllo != '\n') {
        if (scanf("%s %d", nome_ingrediente, &qta)) {}

        hash = hashing(nome_ingrediente, M_HASH_TABLE_LOTTI);
        lista_lotto = lotti_hashTable[hash];

        lotto_doppione = lista_lotti_search(lista_lotto, nome_ingrediente);
        if (lotto_doppione == NULL) {
            lotto = lista_lotti_creazione_nodo(nome_ingrediente, 0);
            lista_lotti_insert(lista_lotto, lotto);
        }
    
        ingrediente = lista_ingredienti_creazione_nodo((lotto_doppione == NULL ? lotto : lotto_doppione), qta);
        lista_ingredienti_insert(lista_ingredienti, ingrediente);
        ricetta->peso += ingrediente->qta;

        controllo = getchar();
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

    x->lotto = lotto;
    x->qta = qta;

    return x;
}

void lista_ingredienti_insert(struct Lista_Ingredienti *ingredienti, struct Ingrediente *x) {
    x->next = ingredienti->head;
    ingredienti->head = x;
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
struct Lotto *lista_lotti_creazione_nodo(char nome[],  int qta) {
    struct Lotto* x = calloc(1, sizeof(struct Lotto));

    x->nome = malloc(strlen(nome) * sizeof(char) + 1);
    strcpy(x->nome, nome);
    x->qta_totale = qta;

    return x;
}
void lista_lotti_insert(struct Lista_Lotti *lotti, struct Lotto *x) {
    x->next = lotti->head;
    lotti->head = x;
}
struct Lotto* lista_lotti_search(struct Lista_Lotti* lotti, char nome[]) {
    struct Lotto* nodo = lotti->head;

    while(nodo && strcmp(nodo->nome, nome))
        nodo = nodo->next;

    return nodo;
}

// --- Albero Lotti ---
struct Foglia_Lotto* albero_lotti_creazione_foglia(int qta, int scadenza) {
    struct Foglia_Lotto* x = calloc(1, sizeof(struct Foglia_Lotto));

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
        if (scanf("%s %d %d", nome_ingrediente, &qta, &scadenza)) {}

        if (tempo < scadenza) {
            hash = hashing(nome_ingrediente, M_HASH_TABLE_LOTTI);
            lista_lotto = lotti_hashTable[hash];

            lotto_doppione = lista_lotti_search(lista_lotto, nome_ingrediente);
            if (lotto_doppione) {
                lotto_doppione->qta_totale += qta;
                if (lotto_doppione->sub_lotti == NULL)
                    lotto_doppione->sub_lotti = calloc(1, sizeof(struct Albero_Lotti));
            } else {
                lotto = lista_lotti_creazione_nodo(nome_ingrediente, qta);
                lista_lotti_insert(lista_lotto, lotto);
                lotto->sub_lotti = calloc(1, sizeof(struct Albero_Lotti));
            }

            sub_lotto = albero_lotti_creazione_foglia(qta, scadenza);
            albero_lotti_insert((lotto_doppione ? lotto_doppione->sub_lotti : lotto->sub_lotti), sub_lotto);
        }

        controllo = getchar();
        memset(nome_ingrediente, 0, sizeof(nome_ingrediente));
    }

    controllo_ordini_attesa(ordini_attesa_lista, ordini_albero, tempo);

    printf("rifornito\n");
}


// FUNZIONE ORDINI
// --- Albero Ordini ---
struct Ordine* albero_ordini_creazione_foglia(struct Ricetta *ricetta, int n_elementi, int data) {
    struct Ordine* x = calloc(1, sizeof(struct Ordine));
    
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

    if (scanf("%s %d", nome_ricetta, &n_elementi)) {}

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

void controllo_ordini_scaduti(struct Lista_Lotti* lotti_hashTable[], int tempo) {
    struct Lotto* lotto = NULL;

    for (int i = 0; i < M_HASH_TABLE_LOTTI; i++) {
        lotto = lotti_hashTable[i]->head;

        while(lotto) {
            if (lotto->sub_lotti)
                ricalcolo_qta_totale(lotto, tempo);
            lotto = lotto->next;
        }
    }
}

struct Ingrediente* ordine_controllo_attesa(struct Ordine_Attesa* ordine_attesa, struct Ricetta* ricetta, int n_elementi, int tempo) {
    int qta_necessaria;
    struct Ingrediente *ingrediente = ricetta->ingredienti->head;
    struct Lotto* lotto;

    if (ordine_attesa != NULL) {
        lotto = ordine_attesa->primo_ingrediente_mancante->lotto;
        qta_necessaria = ordine_attesa->primo_ingrediente_mancante->qta * n_elementi;

        if (lotto->qta_totale < qta_necessaria)
            return ingrediente;
    }

    while(ingrediente) {
        lotto = ingrediente->lotto;
        qta_necessaria = ingrediente->qta * n_elementi;

        if (lotto->sub_lotti == NULL || lotto->qta_totale < qta_necessaria)
            return ingrediente;

        ingrediente = ingrediente->next;
    }

    return NULL;
}

int preparazione_ordine(struct Lista_Ordini_Attesa* ordini_attesa_lista, struct Ricetta* ricetta, int n_elementi, int tempo) {
    struct Ingrediente* attesa;
    struct Ordine_Attesa* ordine_attesa;

    attesa = ordine_controllo_attesa(NULL, ricetta, n_elementi, tempo);

    if (attesa != NULL) {
        ordine_attesa = lista_ordini_attesa_creazione_nodo(n_elementi, tempo, ricetta, attesa);
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
    struct Ingrediente* attesa;
    struct Ordine_Attesa* ordine_attesa = ordini_attesa_lista->head;
    struct Ordine* ordine;

    while(ordine_attesa) {
        attesa = ordine_controllo_attesa(ordine_attesa, ordine_attesa->ricetta, ordine_attesa->n_elementi, tempo);

        if (attesa == NULL) {
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
struct Ordine_Attesa* lista_ordini_attesa_creazione_nodo(int n_elementi, int data, struct Ricetta* ricetta, struct Ingrediente* ingrediente_mancante) {
    struct Ordine_Attesa *x = calloc(1, sizeof(struct Ordine_Attesa));

    x->data = data;
    x->n_elementi = n_elementi;
    x->ricetta = ricetta;
    x->primo_ingrediente_mancante = ingrediente_mancante;

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