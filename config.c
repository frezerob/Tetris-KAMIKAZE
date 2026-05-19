#include "config.h"
#include <stdio.h>   // FILE, fprintf, fscanf, fopen, fclose
#include <stdlib.h>  // atoi
#include <string.h>  // strcmp

TDAconfig config;

void ConfigAplicarResolucion()
{
    if(config.ANCHO == 320 && config.ALTO == 200){
        config.ESCALA = 3;
        config.TAM_CELDA = 8;
        config.ESCALA_FUENTE = 1;
    }
    else if(config.ANCHO == 640 && config.ALTO == 480){
        config.ESCALA = 1;
        config.TAM_CELDA = 20;
        config.ESCALA_FUENTE = 3;
    }
}

int ConfigCargar(char* archivo)
{
    FILE* f = fopen(archivo, "r");
    if(!f){
        // No existe, creamos con valores por defecto
        config.ANCHO = 320;
        config.ALTO  = 200;
        config.DIFICULTAD = 1;  // NORMAL
        config.PALETA = 0;
        ConfigAplicarResolucion();
        ConfigGuardar(archivo);
        return 0;
    }

    char clave[32], valor[32];
    while(fscanf(f, "%s %s", clave, valor) == 2){
        if(strcmp(clave, "DIFICULTAD") == 0){
            if(strcmp(valor, "FACIL")   == 0) config.DIFICULTAD = 0;
            if(strcmp(valor, "NORMAL")  == 0) config.DIFICULTAD = 1;
            if(strcmp(valor, "DIFICIL") == 0) config.DIFICULTAD = 2;
        }
        else if(strcmp(clave, "RESOLUCION") == 0){
            config.ANCHO = atoi(valor);
            fscanf(f, "%s", valor);
            config.ALTO = atoi(valor);
        }
        else if(strcmp(clave, "PALETA") == 0){
            config.PALETA = atoi(valor);
        }
    }

    fclose(f);
    ConfigAplicarResolucion();
    return 0;
}

int ConfigGuardar(char* archivo)
{
    FILE* f = fopen(archivo, "w");
    if(!f) return -1;

    char* difs[] = {"FACIL", "NORMAL", "DIFICIL"};

    fprintf(f, "DIFICULTAD %s\n", difs[config.DIFICULTAD]);
    fprintf(f, "RESOLUCION %d %d\n", config.ANCHO, config.ALTO);
    fprintf(f, "PALETA %d\n", config.PALETA);

    fclose(f);
    return 0;
}
