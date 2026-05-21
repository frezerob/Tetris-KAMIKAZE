#include "core.h"
#include "menu.h"
#include "graficos.h"
#include <stdio.h>
#include <string.h>


int MenuIniciar(TDAconfig cfg)
{
    char* opciones_menu[] = {"JUGAR", "CONFIGURACION", "SALIR"};
    int cant = 3;
    int opcion = 0;
    eGBT_Tecla tecla;

    while(1){
        ImprimirMenu(opcion, opciones_menu, cant);

        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();

        if(tecla == GBTK_w)
            opcion = (opcion + cant - 1) % cant;
        if(tecla == GBTK_s)
            opcion = (opcion + 1) % cant;

        if(tecla == GBTK_ENTER){
            if(opcion == 0) return 0;
            if(opcion == 1) MenuConfiguracion();
            if(opcion == 2) return SALIR;
        }

        if(tecla == GBTK_ESCAPE)
            return SALIR;

        gbt_esperar(16);
    }
}

void MenuConfiguracion()
{
    // char* opcMenu[] = {"DIFICULTAD", "RESOLUCION", "VOLVER"}; ???
    int cant = 3;
    int opcion = 0;
    int corriendo = 1;
    eGBT_Tecla tecla;
    char* difs[] = {"FACIL", "NORMAL", "DIFICIL"};
    char* ress[] = {"320X200", "640X480"};

    while(corriendo){
        // Mostramos la opcion seleccionada actualmente en cada fila
        char lineaDif[32], lineaRes[32];
        sprintf(lineaDif, "DIFICULTAD %s", difs[config.DIFICULTAD]);
        sprintf(lineaRes, "RESOLUCION %s", ress[config.ANCHO == 320 ? 0 : 1]);

        char* opcMostrar[] = {lineaDif, lineaRes, "VOLVER"};
        ImprimirMenu(opcion, opcMostrar, cant);

        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();

        if(tecla == GBTK_s || tecla == GBTK_ABAJO)
            opcion = (opcion + 1) % cant;
        if(tecla == GBTK_w || tecla == GBTK_ARRIBA)
            opcion = (opcion + cant - 1) % cant;

        if(tecla == GBTK_ENTER){
            if(opcion == 0){
                config.DIFICULTAD = (config.DIFICULTAD + 1) % 3;
                ConfigGuardar(CONFIG_FILE);
            }
            else if(opcion == 1){
                if(config.ANCHO == 320){
                    config.ANCHO = 640;
                    config.ALTO  = 480;
                }
                else{
                    config.ANCHO = 320;
                    config.ALTO  = 200;
                }
                ConfigAplicarResolucion();
                config.OFFSET_X = (config.ANCHO - (COL_TABLERO * config.TAM_CELDA)) / 8;
                config.OFFSET_Y = (config.ALTO  - (FIL_TABLERO * config.TAM_CELDA)) / 2;
                ConfigGuardar(CONFIG_FILE);

                gbt_destruir_ventana();
                gbt_crear_ventana(TITULO, config.ANCHO, config.ALTO, config.ESCALA);
            }
            else if(opcion == 2)
                corriendo = 0;
        }
        if(tecla == GBTK_ESCAPE)
            corriendo = 0;
    }
}
int MenuGameOver(int puntaje)
{
    char* opciones[] = {"REINICIAR", "MENU PRINCIPAL"};
    int cant = 2;
    int opcion = 0;
    eGBT_Tecla tecla;

    while(1){
        gbt_borrar_backbuffer(N);

        // Dibujamos todo antes de volcar
        DibujarTextoCentrado("GAME OVER", config.ALTO / 4, W);
        DibujarPuntaje(puntaje, (config.ANCHO - CalcularAnchoTexto("000000")) / 2, config.ALTO / 4 + 20, 4);

        // Dibujamos las opciones manualmente sin usar ImprimirMenu
        int Y = config.ALTO / 2;
        for(int i = 0; i < cant; i++){
            if(i == opcion)
                DibujarTextoCentrado(opciones[i], Y, W);
            else
                DibujarTextoCentrado(opciones[i], Y, 4);
            Y += config.ALTO / (cant * 2);
        }

        gbt_volcar_backbuffer();

        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();

        if(tecla == GBTK_w)
            opcion = (opcion + cant - 1) % cant;
        if(tecla == GBTK_s)
            opcion = (opcion + 1) % cant;
        if(tecla == GBTK_ENTER){
            if(opcion == 0) return REINICIAR;
            if(opcion == 1) return MENU_PRINCIPAL;
        }

        gbt_esperar(16);
    }
}
