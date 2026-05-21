#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "core.h"

#define SALIR -2
#define REINICIAR       1
#define MENU_PRINCIPAL  2

int MenuIniciar(TDAconfig config);
void MenuConfiguracion();
int MenuGameOver(int puntaje);
#endif // MENU_H_INCLUDED
