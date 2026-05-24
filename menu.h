#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "core.h"
#include "config.h"
#define SALIR -2
#define REINICIAR       1
#define MENU_PRINCIPAL  2
#define REANUDAR 3

int MenuIniciar(TDAconfig config);
void MenuConfiguracion();
int MenuGameOver(int puntaje);
int8_t MenuPausa();
void CalcularOpcion(eGBT_Tecla *tecla, uint8_t *opcion, uint8_t cantidad_opciones);
char* PantallaIngresoNombre();
#endif // MENU_H_INCLUDED
