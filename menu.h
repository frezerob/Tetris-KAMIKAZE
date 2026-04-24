#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//MACROS
#define TITULO "Primera Prueba"
#define ANCHO 400
#define ALTO 250
#define ESCALA 2
#define TILE_SIZE 10 // Tamaño de la tile

//ERRORES
#define INIT_ERROR -1
#define COLOR_ERROR -2
#define WINDOW_ERROR -3

uint8_t IniciarMenu();

#endif // MENU_H_INCLUDED
