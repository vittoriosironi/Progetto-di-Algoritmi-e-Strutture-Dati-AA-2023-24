#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 255
#define AGGIUNGI_RICETTA "AGGIUNGI_RICETTA"
#define RIMUOVI_RICETTA "RIMUOVI_RICETTA"
#define RIFORNIMENTO "RIFORNIMENTO"
#define ORDINE "ORDINE"

int PERIODO_CORRIERE, CAPIENZA_CORRIERE;

int main() {
    int pass = 1, tempo = 0;
    char input[MAX_LENGTH];

    scanf("%d %d", &PERIODO_CORRIERE, &CAPIENZA_CORRIERE);

    while(pass) { // -> DA SISTEMARE
        printf("%d\n", tempo);
        pass = scanf("%s ", input);

        if (pass) {
            printf("%s \n", input);

            if (strcmp(input, AGGIUNGI_RICETTA)) {

            } else if (strcmp(input, RIMUOVI_RICETTA)) {

            } else if (strcmp(input, RIFORNIMENTO)) {

            } else if (strcmp(input, ORDINE)) {

            } else { tempo -= 1; }
            tempo += 1;
        }
    }

    return 0;
}