#include "core.h"
#include "menu.h"
#include "graficos.h"
#include "score.h"
#include "partida.h"
#include <stdio.h>
#include <string.h>


void CalcularOpcion(eGBT_Tecla *tecla, uint8_t *opcion, uint8_t cantidad_opciones)
{
    *tecla = gbt_obtener_tecla_presionada();
    if(*tecla == GBTK_w)
        *opcion = (*opcion + cantidad_opciones - 1) % cantidad_opciones;
    if(*tecla == GBTK_s)
        *opcion = (*opcion + 1) % cantidad_opciones;
}

int MenuIniciar(TDAconfig cfg)
{
    int hayPartida = PartidaExiste();
    char* sinPartida[] = {"JUGAR", "ESTADISTICAS", "CONFIGURACION", "SALIR"};
    char* conPartida[] = {"JUGAR", "CONTINUAR", "ESTADISTICAS", "CONFIGURACION", "SALIR"};
    char** opciones = hayPartida ? conPartida : sinPartida;
    uint8_t cant = hayPartida ? 5 : 4;
    uint8_t opcion = 0;
    eGBT_Tecla tecla;

    while(1){
        ImprimirMenu(opcion, opciones, cant);
        gbt_procesar_entrada();
        CalcularOpcion(&tecla, &opcion, cant);

        if(tecla == GBTK_ENTER){
            if(opcion == 0) return 0;
            if(hayPartida && opcion == 1) return CONTINUAR;
            if(opcion == (hayPartida ? 2 : 1)) MenuEstadisticas();
            if(opcion == (hayPartida ? 3 : 2)) MenuConfiguracion();
            if(opcion == (hayPartida ? 4 : 3)) return SALIR;
        }
        if(tecla == GBTK_ESCAPE)
            return SALIR;

        gbt_esperar(16);
    }
}

void MenuConfiguracion()
{
    uint8_t cant = 6;
    uint8_t opcion = 0;
    uint8_t corriendo = 1;
    char lineaPaleta[32];
    eGBT_Tecla tecla;
    char* difs[] = {"FACIL", "NORMAL", "DIFICIL"};
    char* ress[] = {"320X200", "640X480"};
    char* nombresPaleta[] = {"ORIGINAL", "LCD", "MONOCROMO"};
    char *modos[] = {"CLASICO", "DELUXE"};
    while(corriendo){
        char lineaDif[32], lineaRes[32], modoJuego[32], colTablero[32];
        sprintf(lineaDif, "DIFICULTAD %s", difs[config.DIFICULTAD]);
        sprintf(lineaPaleta, "PALETA %s", nombresPaleta[config.PALETA]);
        sprintf(lineaRes, "RESOLUCION %s", ress[config.ANCHO == 640 ? 0 : 1]);
        sprintf(modoJuego, "MODO DE JUEGO %s",modos[config.MODO]);
        sprintf(colTablero,"CANTIDAD DE COLUMNAS %d",config.COL_TABLERO);
        char* opcMostrar[] = {lineaDif, lineaRes, modoJuego, colTablero, lineaPaleta, "VOLVER"};


        gbt_procesar_entrada();
        CalcularOpcion(&tecla, &opcion, cant);
        ImprimirMenu(opcion, opcMostrar, cant);

        if(tecla == GBTK_ENTER){
            if(opcion == 0){
                config.DIFICULTAD = (config.DIFICULTAD + 1) % 3;
                ConfigGuardar(CONFIG_FILE);
            }
            else if(opcion == 1){
                if(config.ANCHO == 320){
                    AplicarConfig(640);
                }
                else if (config.ANCHO == 640){
                    AplicarConfig(320);
                }
                config.OFFSET_X = 2 * config.TAM_CELDA;
                config.OFFSET_Y = 4 * config.TAM_CELDA;
                ConfigGuardar(CONFIG_FILE);
                gbt_destruir_ventana();
                gbt_crear_ventana(TITULO, config.ANCHO, config.ALTO, config.ESCALA);
            }
            else if(opcion == 2){
                config.MODO = (config.MODO + 1) % 2;
                config.COL_TABLERO = (config.MODO == CLASICO) ? 10 : 8;
            }
            else if(opcion == 3 && config.MODO == DELUXE){
                config.COL_TABLERO++;
                if(config.COL_TABLERO > 16)
                    config.COL_TABLERO = 8;
            }
            else if(opcion == 4){
                config.PALETA = (config.PALETA + 1) % 3;
                AplicarPaleta(config.PALETA);
                ConfigGuardar(CONFIG_FILE);
            }
            else if(opcion == 5)
                corriendo = 0;
        }
        if(tecla == GBTK_ESCAPE)
            corriendo = 0;
        gbt_esperar(16);
    }
}


int MenuGameOver(int puntaje)
{
    char* opciones[] = {"REINICIAR", "MENU PRINCIPAL"};
    uint8_t cant = 2;
    uint8_t opcion = 0;
    eGBT_Tecla tecla;
    char spuntaje[16];
    itoa(puntaje,spuntaje,10);
    while(1){
        gbt_borrar_backbuffer(N);
        DibujarFondo();

        // Dibujamos todo antes de volcar
        DibujarTextoCentradoConSombra("GAME OVER", config.ALTO / 4,O,0,N);
        DibujarTextoCentradoConSombra(spuntaje,config.OFFSET_Y,O,0,N);

        // Dibujamos las opciones manualmente sin usar ImprimirMenu
        uint16_t Y = config.ALTO / 2;
        for(uint8_t i = 0; i < cant; i++){
            if(i == opcion)
                DibujarTextoCentradoConSombra(opciones[i], Y, W,0,N);
            else
                DibujarTextoCentradoConSombra(opciones[i], Y, 4,0,N);
            Y += config.ALTO / (cant * 2);
        }

        gbt_volcar_backbuffer();

        gbt_procesar_entrada();

        CalcularOpcion(&tecla,&opcion,cant);

        if(tecla == GBTK_ENTER){
            if(opcion == 0) return REINICIAR;
            if(opcion == 1) return MENU_PRINCIPAL;
        }

        gbt_esperar(16);
    }
}

int8_t MenuPausa()
{
    eGBT_Tecla tecla;
    uint8_t opcion = 0;
    char* opciones[] = {"REANUDAR", "GUARDAR Y SALIR", "SALIR SIN GUARDAR"};

    while(1){
        ImprimirMenu(opcion, opciones, 3);
        gbt_procesar_entrada();
        CalcularOpcion(&tecla, &opcion, 3);

        if(tecla == GBTK_ENTER){
            if(opcion == 0) return REANUDAR;
            if(opcion == 1) return GUARDAR_Y_SALIR;
            if(opcion == 2) return SALIR;
        }
        if(tecla == GBTK_ESCAPE)
            return REANUDAR;

        gbt_esperar(16);
    }
}

char* PantallaIngresoNombre()
{
    char *nombre = calloc(4, sizeof(char));
    uint8_t CantidadChar = 0;
    eGBT_Tecla tecla;
    while(TRUE)
    {
        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();

        if(tecla >= GBTK_a && tecla <= GBTK_z && CantidadChar < 3)
        {
            // Si GBTK_a equivale a la 'A' o 'a' física, calculamos el desplazamiento:
            char caracter = (char)tecla - 32;

            nombre[CantidadChar] = caracter;
            CantidadChar++;
            nombre[CantidadChar] = '\0'; // Aseguramos siempre el fin de string
        }
        else if(tecla == GBTK_RETROCESO){
            if(CantidadChar > 0){
                CantidadChar--;
                nombre[CantidadChar] = '\0';
            }
        }

        if(tecla == GBTK_ENTER && CantidadChar==MAX_NOMBRE)
            return nombre;

        gbt_borrar_backbuffer(N);
        DibujarFondo();
        DibujarTextoCentradoConSombra("INGRESE SU NOMBRE",config.OFFSET_Y*2,O,0,N);
        DibujarTextoCentradoConSombra(nombre,config.OFFSET_Y*3,O,0,N);
        gbt_volcar_backbuffer();

        //Espera para no sobrecargar CPU
        gbt_esperar(16);
    }

}

void MenuEstadisticas()
{
    Score scores[MAX_SCORES];
    int cant = 0;
    ScoresCargar(scores, &cant);

    eGBT_Tecla tecla;
    char linea[32];
    int espaciado = TAM_FUENTE8X8 * config.ESCALA_FUENTE + 4;

    while(1){
        gbt_borrar_backbuffer(N);
        DibujarFondo();

        DibujarTextoCentradoConSombra("TOP 10", config.OFFSET_Y, O, 0, N);

        for(int i = 0; i < cant; i++){
            sprintf(linea, "%d %s %d", i + 1, scores[i].nombre, scores[i].puntaje);
            int Y = config.OFFSET_Y + espaciado * 2 + i * espaciado;
            if(i == 0)
                DibujarTextoCentradoConSombra(linea, Y, O, 0, N);
            else
                DibujarTextoCentradoConSombra(linea, Y, W, 0, N);
        }

        if(cant == 0)
            DibujarTextoCentradoConSombra("SIN REGISTROS", config.ALTO / 2, W, 0, N);

        DibujarTextoCentradoConSombra("ENTER PARA VOLVER", config.ALTO - espaciado * 2, BRD, 0, N);

        gbt_volcar_backbuffer();

        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();

        if(tecla == GBTK_ENTER || tecla == GBTK_ESCAPE)
            break;

        gbt_esperar(16);
    }
}
