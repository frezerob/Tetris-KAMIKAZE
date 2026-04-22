#include <stdio.h>
#include <stdlib.h>
#include "GBT/gbt.h"

//CONSTANTES
#define TITULO "Primera Prueba"
#define ANCHO 25
#define ALTO 25
#define ESCALA 15


//ERRORES
#define INIT_ERROR -1
#define COLOR_ERROR -2
#define WINDOW_ERROR -3

int main()
{
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

    uint8_t corriendo = 1;
    eGBT_Tecla tecla; // Estructura tecla que detecta las teclas presionadas
    while(corriendo)
    {
        gbt_procesar_entrada(); // Actualiza el estado de las teclas
        tecla = gbt_obtener_tecla_presionada(); // Carga en la estructura tecla la tecla presionada
        if(tecla == GBTK_ESCAPE)
            corriendo = 0;



    }


    return 0;
}
