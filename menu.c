#include "core.h"
#include "menu.h"
#include "graficos.h"
#include <stdio.h>
#include <string.h>


int MenuIniciar(TDAconfig cfg)
{
    tGBT_ColorRGB paleta[] = {
        {0x00, 0x00, 0x00}, // N -> Negro (Fondo)

        // I - CIAN
        {0x00, 0xD0, 0xD0}, // I        -> Base (Cian medio)
        {0x00, 0x7A, 0x7A}, // I_OSCURO -> Sombra
        {0x80, 0xFF, 0xFF}, // I_CLARO  -> Brillo

        // O - AMARILLO
        {0xD0, 0xD0, 0x00}, // O        -> Base
        {0x7A, 0x7A, 0x00}, // O_OSCURO -> Sombra
        {0xFF, 0xFF, 0x80}, // O_CLARO  -> Brillo

        // T - PÚRPURA
        {0xC0, 0x00, 0xC0}, // T        -> Base
        {0x70, 0x00, 0x70}, // T_OSCURO -> Sombra
        {0xFF, 0x80, 0xFF}, // T_CLARO  -> Brillo

        // S - VERDE
        {0x00, 0xC0, 0x00}, // S        -> Base
        {0x00, 0x70, 0x00}, // S_OSCURO -> Sombra
        {0x80, 0xFF, 0x80}, // S_CLARO  -> Brillo

        // Z - ROJO
        {0xD0, 0x00, 0x00}, // Z        -> Base
        {0x7A, 0x00, 0x00}, // Z_OSCURO -> Sombra
        {0xFF, 0x80, 0x80}, // Z_CLARO  -> Brillo

        // J - AZUL
        {0x22, 0x55, 0xFF}, // J        -> Base
        {0x00, 0x22, 0xAA}, // J_OSCURO -> Sombra
        {0x99, 0xBB, 0xFF}, // J_CLARO  -> Brillo

        // L - NARANJA
        {0xE6, 0x73, 0x00}, // L        -> Base
        {0x8F, 0x47, 0x00}, // L_OSCURO -> Sombra
        {0xFF, 0xB3, 0x66}, // L_CLARO  -> Brillo

        {0x80, 0x80, 0x80}, // BRD -> Gris Borde
        {0x40, 0x40, 0x40}, // BRD_OSCURO -> Gris Borde
        {0xB0, 0xB0, 0xB0}, // BRD_CLARO -> Gris Borde
        {0xFF, 0xFF, 0xFF}, // W   -> Blanco
        {0x01, 0x01, 0x01}  // TR  -> Transparente
    };
    if(gbt_iniciar() != 0){
        printf("%s", gbt_obtener_log());
        return INIT_ERR;
    }
    if(gbt_crear_ventana(TITULO, cfg.ANCHO, cfg.ALTO, cfg.ESCALA) != 0){
        printf("%s", gbt_obtener_log());
        return WINDOW_ERR;
    }
    if(gbt_aplicar_paleta(paleta, PALETA_MAX_COLORES, GBT_FORMATO_888) != 0){
        printf("%s", gbt_obtener_log());
        return COLOR_ERR;
    }

    char* opciones_menu[] = {"JUGAR", "CONFIGURACION", "SALIR"};
    int cant = 3;
    int opcion = 0;       // inicializada en 0
    eGBT_Tecla tecla;

    while(TRUE){
        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();

        if(tecla == GBTK_w)
            opcion = (opcion + cant - 1) % cant;
        if(tecla == GBTK_s)
            opcion = (opcion + 1) % cant;

        if(tecla == GBTK_ENTER){
            if(opcion == 0) return 0;           // JUGAR
            if(opcion == 1) MenuConfiguracion(); // CONFIGURACION
            if(opcion == 2) return SALIR;        // SALIR
        }

        ImprimirMenu(opcion, opciones_menu, cant);
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


