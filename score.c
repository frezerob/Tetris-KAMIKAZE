#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "score.h"

void ScoresCargar(Score scores[], int* cant)
{
    *cant = 0;
    FILE* f = fopen(SCORES_FILE, "r");
    if(!f) return;

    while(*cant < MAX_SCORES &&
          fscanf(f, "%s %d", scores[*cant].nombre, &scores[*cant].puntaje) == 2)
        (*cant)++;

    fclose(f);
}

void ScoresGuardar(Score scores[], int cant)
{
    FILE* f = fopen(SCORES_FILE, "w");
    if(!f) return;

    for(int i = 0; i < cant; i++)
        fprintf(f, "%s %d\n", scores[i].nombre, scores[i].puntaje);

    fclose(f);
}

void ScoresAgregar(Score scores[], int* cant, char* nombre, int puntaje)
{
    if(*cant < MAX_SCORES || puntaje > scores[*cant - 1].puntaje){
        int pos = *cant < MAX_SCORES ? *cant : MAX_SCORES - 1;

        for(int i = 0; i < *cant && i < MAX_SCORES; i++){
            if(puntaje > scores[i].puntaje){
                pos = i;
                break;
            }
        }

        for(int i = MAX_SCORES - 1; i > pos; i--)
            scores[i] = scores[i-1];

        strncpy(scores[pos].nombre, nombre, MAX_NOMBRE);
        scores[pos].puntaje = puntaje;

        if(*cant < MAX_SCORES) (*cant)++;
        ScoresGuardar(scores, *cant);
    }
}
