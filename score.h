#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED
#include "core.h"

#define MAX_SCORES 10
#define SCORES_FILE "scores.txt"

typedef struct {
    char nombre[MAX_NOMBRE];
    int puntaje;
} Score;

void ScoresCargar(Score scores[], int* cant);
void ScoresGuardar(Score scores[], int cant);
void ScoresAgregar(Score scores[], int* cant, char* nombre, int puntaje);

#endif // SCORE_H_INCLUDED
