#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CONST_HASH 0.6180339887 // (sqrt(5) - 1) / 2
#define M_HASH_TABLE 1024 // valore di m = 2^p (potenza di 10)
/*
unsigned int normalize(char *key) {
    int hash = 0, size = strlen(key);
    char normalize_key[size], temp[3];

    for(int i = 0; key[i] != '\0'; i++) {
        sprintf(temp, "%d", key[i]);
        strcat(normalize_key, (char*)temp);
        printf("%d", key[i]);
    }

    printf("\n");
    sprintf(temp, "%d", *key);
    strcat(normalize_key, temp);
    sprintf(temp, "%d", *(key+1));
    strcat(normalize_key, temp);

    return atoi(normalize_key);
}*/

// DA CAMBIARE
unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

float decimal(float num) { return num - (int) num; }
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


int main() {
    char key[256];
    scanf("%s", key);
    printf("%u", hashing(key));

    return 0;
}