#include "core.h"
#include "menu.h"
#include "graficos.h"

int8_t MenuIniciar(TDAconfig config)
{
    tGBT_ColorRGB paleta[] = {
        {0x00, 0x00, 0x00}, // N   -> NEGRO
        {0x00, 0xFF, 0xFF}, // I   -> CIAN
        {0xFF, 0xFF, 0x00}, // O   -> AMARILLO
        {0x80, 0x00, 0x80}, // T   -> PÚRPURA
        {0x00, 0xFF, 0x00}, // S   -> VERDE
        {0xFF, 0x00, 0x00}, // Z   -> ROJO
        {0x00, 0x00, 0xFF}, // J   -> AZUL
        {0xFF, 0xA5, 0x00}, // L   -> NARANJA
        {0x80, 0x80, 0x80}, // BRD -> GRIS (Borde)
        {0xFF, 0xFF, 0xFF}, // W   -> BLANCO
        {0x01, 0x01, 0x01}  // TR  -> TRANSPARENTE
    };
    if(gbt_iniciar() != 0){
        printf("%s",gbt_obtener_log());
        return INIT_ERR;
    }
    //CREAMOS LA VENTANA
    if(gbt_crear_ventana(TITULO,config.ANCHO,config.ALTO,config.ESCALA) != 0){
        printf("%s",gbt_obtener_log());
        return WINDOW_ERR;
    }

    if(gbt_aplicar_paleta(paleta,PALETA_MAX_COLORES,GBT_FORMATO_888)!=0){
        printf("%s",gbt_obtener_log());
        return COLOR_ERR;

    }
    eGBT_Tecla tecla;
    while(TRUE)
    {
        char* opciones_menu[]={
        "JUGAR",
        "CONFIGURACION",
        "SALIR"
        };

        size_t cant_opciones = sizeof(opciones_menu) / sizeof(opciones_menu[0]);
        uint8_t opcion;
        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();
        if(tecla == GBTK_w)
            opcion = (opcion+2)%cant_opciones;
        if(tecla == GBTK_s)
            opcion = (opcion+1) % cant_opciones;
        ImprimirMenu(opcion, opciones_menu, cant_opciones);
        if(tecla == GBTK_ENTER)
            break;
    }

    return 0;
}


