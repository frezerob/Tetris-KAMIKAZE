#include "core.h"
#include "menu.h"
#include "pixdraw.h"


int8_t MenuIniciar()
{
    uint8_t Example[15][15] =
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,0,1,1,1,0,1,1,1,0,1,1,1},
        {0,1,0,0,1,0,0,0,0,1,0,0,1,0,1},
        {0,1,0,0,1,1,0,0,0,1,0,0,1,1,0},
        {0,1,0,0,1,0,0,0,0,1,0,0,1,0,1},
        {0,1,0,0,1,1,1,0,0,1,0,0,1,0,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,1,1,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,1,1,0,0,0,0,0,0,0,0,0},
        {0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,1,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    if(gbt_iniciar() != 0){
        printf("%s",gbt_obtener_log());
        return INIT_ERROR;
    }
    //CREAMOS LA VENTANA
    if(gbt_crear_ventana(TITULO,ANCHO,ALTO,ESCALA) != 0){
        printf("%s",gbt_obtener_log());
        return WINDOW_ERROR;
    }

    if(gbt_aplicar_paleta(NULL,PALETA_MAX_COLORES,GBT_FORMATO_888)!=0){
        printf("%s",gbt_obtener_log());
        return COLOR_ERROR;
    }

    DrawMatrix(Example,15,15);

    return 0;
}


