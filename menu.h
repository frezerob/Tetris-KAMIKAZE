#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "core.h"

#define SALIR -2
#define REINICIAR       1
#define MENU_PRINCIPAL  2
#define REANUDAR 3

int MenuIniciar(TDAconfig config);
void MenuConfiguracion();
int MenuGameOver(int puntaje);
int8_t MenuPausa();
char* PantallaIngresoNombre();
#endif // MENU_H_INCLUDED
