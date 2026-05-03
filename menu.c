#include "core.h"
#include "menu.h"


int8_t MenuIniciar()
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
        {0x40, 0x40, 0x40}, // BRD -> GRIS (Borde)
        {0xFF, 0xFF, 0xFF}, // W   -> BLANCO
        {0x01, 0x01, 0x01}  // TR  -> TRANSPARENTE
    };
    if(gbt_iniciar() != 0){
        printf("%s",gbt_obtener_log());
        return INIT_ERR;
    }
    //CREAMOS LA VENTANA
    if(gbt_crear_ventana(TITULO,ANCHO,ALTO,ESCALA) != 0){
        printf("%s",gbt_obtener_log());
        return WINDOW_ERR;
    }

    if(gbt_aplicar_paleta(paleta,PALETA_MAX_COLORES,GBT_FORMATO_888)!=0){
        printf("%s",gbt_obtener_log());
        return COLOR_ERR;
    }



    return 0;
}


