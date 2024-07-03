#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define CONST_HASH 0.6180339887 // (sqrt(5) - 1) / 2
#define M_HASH_TABLE 991
#define MAX_LENGTH 256
#define AGGIUNGI_RICETTA "aggiungi_ricetta"
#define RIMUOVI_RICETTA "rimuovi_ricetta"
#define RIFORNIMENTO "rifornimento"
#define ORDINE "ordine"

int PERIODO_CORRIERE, CAPIENZA_CORRIERE;

// -- Ingredienti Lista ---
struct Ingrediente {
    struct Ingrediente *prev, *next;
    char nome[MAX_LENGTH];
    int qta;
};

// --- Ricetta Lista ---
struct Ricetta {
    struct Ricetta *prev, *next;
    char nome[MAX_LENGTH];
    struct Ingrediente *ingredienti;
    int peso;
};

// --- UTILITIES ---
void print_error() { printf("ERROR\n"); }
int allocazione_memoria(struct Ricetta *ricette_hashTable[]) {
    for(int i = 0; i < M_HASH_TABLE; i++) {
        ricette_hashTable[i] =  malloc(sizeof(struct Ricetta));
        if (ricette_hashTable[i] == NULL)
            return 1;
    }
    return 0;
}
void free_memoria(struct Ricetta *ricette_hashTable[]) {
    for(int i = 0; i < M_HASH_TABLE; i++)
        free(ricette_hashTable[i]);
}
float decimal(float num) { return num - (int) num; }



// --- STRUTTURE ---
// --- Hash Table ---
/*
unsigned int normalize(char *key) {
    int num = 0;
    for (int i = 0; key[i] != '\0'; i++)
        num += (int) key[i];
    return num;
}

unsigned int hashing(char *key) {
    int normalize_key = normalize(key);
    return (int) ( M_HASH_TABLE * decimal(normalize_key * CONST_HASH));
}
*/
unsigned int hashing(const char *str) {
    unsigned long hash = 5381;
    int i = 0;

    while (str[i] != '\0') {
        hash = hash * 33 + str[i];
        i++;
    }

    return (unsigned int) (hash % (unsigned long) M_HASH_TABLE);
}


// --- Gestione aggiunta ricetta ---
// aggiungi_ricetta <nome_ricetta> <nome_ingrediente> <quantità> ...
void aggiunta_ricetta(struct Ricetta *ricette_hashTable[]) {
    unsigned int hash;
    int pass = 1, qta = 0;
    char nome_ricetta[MAX_LENGTH], nome_ingrediente[MAX_LENGTH];
    struct Ingrediente* temp;

    scanf("%s", nome_ricetta);
    hash = hashing(nome_ricetta);

    if (1) {
        // caso in cui la lista è vuota
    } else {
        // caso in cui la lista è piena
    }
    strcpy(ricette_hashTable[hash]->nome, nome_ricetta);

    for (int i = 0; pass; i++) {
        pass = scanf("%s", nome_ingrediente);

        if(pass) {
            pass = 1;
            pass = scanf("%d", qta);

            if (pass) {
                ricette_hashTable[hash]->
            }
        }
    }



    print_error();
}

int main() {
    FILE * retIn = freopen("./Casi-Test/test.txt", "r", stdin);
    FILE * retOut = freopen("./out.txt", "w", stdout);

    if(retIn == NULL || retOut == NULL) {
        print_error();
        return 1;
    }

    struct Ricetta *ricette_hashTable[M_HASH_TABLE];

    int pass = 1, tempo = 0, error = 0;
    char input[MAX_LENGTH];

    error = allocazione_memoria(ricette_hashTable);
    if (error) {
        print_error();
        return 1;
    }

    scanf("%d %d", &PERIODO_CORRIERE, &CAPIENZA_CORRIERE);

    for(tempo = 0; pass; tempo++) { // -> DA SISTEMARE
        printf("%d\n", tempo);
        pass = scanf("%s", input);

        if (pass) {
            // printf("%s \n", input);

            if (!strcmp(input, AGGIUNGI_RICETTA)) {
                aggiunta_ricetta(ricette_hashTable);
            } else if (!strcmp(input, RIMUOVI_RICETTA)) {

            } else if (!strcmp(input, RIFORNIMENTO)) {

            } else if (!strcmp(input, ORDINE)) {

            } else { tempo--; }
        }
    }

    free_memoria(ricette_hashTable);

    return 0;
}