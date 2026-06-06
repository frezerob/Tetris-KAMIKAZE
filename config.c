#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TDAconfig config;

/*
* Evalúa los valores actuales de ancho y alto guardados en la estructura
* global 'config' y aplica la configuración correspondiente si coinciden
* con las resoluciones soportadas (320x200 o 640x480).
*/
void ConfigAplicarResolucion()
{
    if(config.ANCHO == 320 && config.ALTO == 200)
        AplicarConfig(320);
    else if(config.ANCHO == 640 && config.ALTO == 480){
        AplicarConfig(640);
    }
}

/*
* Inicializa la configuración del sistema. Si se pasan suficientes argumentos
* por línea de comandos (argc >= 4), prioriza estos valores (ancho, alto y escala)
* y los guarda. De lo contrario, intenta leer la configuración desde un archivo
* de texto plano parseando pares clave-valor. Si el archivo no existe, aplica
* una configuración por defecto (320) y la guarda.
*
* archivo: Ruta del archivo de configuración a leer.
* argc: Cantidad de argumentos pasados por consola.
* argv: Vector de cadenas de texto con los argumentos de la consola.
*
* Retorna: 1 si se cargó exitosamente por línea de comandos, 0 si se cargó
* desde el archivo o si se usó la configuración por defecto.
*/
int ConfigCargar(char* archivo, int argc, char* argv[])
{
    if (argc >= 4) {
        config.ANCHO = atoi(argv[1]);
        config.ALTO = atoi(argv[2]);

        ConfigAplicarResolucion();

        config.ESCALA = atoi(argv[3]);

        ConfigGuardar(archivo);
        return 1;
    }

    FILE* f = fopen(archivo, "r");
    if(!f){
        AplicarConfig(320);
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
        else if(strcmp(clave, "COL_TABLERO") == 0)
            config.COL_TABLERO = atoi(valor);
    }

    fclose(f);
    AplicarConfig(config.ANCHO);

    return 0;
}

/*
* Exporta el estado actual de la estructura global 'config' y lo escribe
* en un archivo de texto con un formato legible (Clave Valor) para que
* pueda ser recuperado en futuras ejecuciones.
*
* archivo: Ruta del archivo donde se guardarán los datos.
*
* Retorna: 0 si el archivo se guardó correctamente, -1 si hubo un error
* al intentar abrir o crear el archivo.
*/
int ConfigGuardar(char* archivo)
{
    FILE* f = fopen(archivo, "w");
    if(!f) return -1;

    char* difs[] = {"FACIL", "NORMAL", "DIFICIL"};

    fprintf(f, "DIFICULTAD %s\n", difs[config.DIFICULTAD]);
    fprintf(f, "RESOLUCION %d %d\n", config.ANCHO, config.ALTO);
    fprintf(f, "PALETA %d\n", config.PALETA);
    fprintf(f, "COL_TABLERO %d\n", config.COL_TABLERO);

    fclose(f);
    return 0;
}

/*
* Ajusta los parámetros internos del entorno gráfico y del tablero (escalas,
* tamaños de celda y fuentes) basándose en el ancho de resolución recibido.
* Si se solicita 640, define valores para alta resolución; para cualquier
* otro valor, define el estándar de baja resolución (320x200). También asegura
* valores por defecto para las dimensiones del tablero.
*
* res: El ancho de la resolución deseada (por ejemplo, 320 o 640).
*/
void AplicarConfig(uint16_t res){
    if(res == 640){
        config.ANCHO = 640;
        config.ALTO = 480;
        config.ESCALA = 1.5;
        config.TAM_CELDA = 16;
        config.ESCALA_FUENTE = 1.8;
        config.FUENTE = fuente_16x8;
    }
    else{
        config.ANCHO = 320;
        config.ALTO = 200;
        config.ESCALA = 2;
        config.TAM_CELDA = 8;
        config.ESCALA_FUENTE = 1;
        config.FUENTE = fuente_8x8;
    }
    config.FIL_TABLERO = 20;
    if(config.COL_TABLERO == 0)
        config.COL_TABLERO = 10;
}
