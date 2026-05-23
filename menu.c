#include "core.h"
#include "menu.h"
#include "graficos.h"
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
    char* opciones_menu[] = {"JUGAR", "CONFIGURACION", "SALIR"};
    uint8_t cant = 3;
    uint8_t opcion = 0;
    eGBT_Tecla tecla;

    while(1){
        ImprimirMenu(opcion, opciones_menu, cant);

        gbt_procesar_entrada();

        CalcularOpcion(&tecla,&opcion,cant);

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
    uint8_t cant = 3;
    uint8_t opcion = 0;
    uint8_t corriendo = 1;
    eGBT_Tecla tecla;
    char* difs[] = {"FACIL", "NORMAL", "DIFICIL"};
    char* ress[] = {"320X200", "640X480"};

    while(corriendo){
        // Mostramos la opcion seleccionada actualmente en cada fila
        char lineaDif[32], lineaRes[32];
        sprintf(lineaDif, "DIFICULTAD %s", difs[config.DIFICULTAD]);
        sprintf(lineaRes, "RESOLUCION %s", ress[config.ANCHO == 640 ? 0 : 1]);

        char* opcMostrar[] = {lineaDif, lineaRes, "VOLVER"};
        ImprimirMenu(opcion, opcMostrar, cant);

        gbt_procesar_entrada();

        CalcularOpcion(&tecla, &opcion, cant);


        // Opcion de cambio de resolucion
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
            else if(opcion == 2)
                break;

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
        DibujarTextoCentrado("GAME OVER", config.ALTO / 4, 4, 0);
        DibujarTextoCentrado(spuntaje,config.OFFSET_Y,O,0);

        // Dibujamos las opciones manualmente sin usar ImprimirMenu
        uint16_t Y = config.ALTO / 2;
        for(uint8_t i = 0; i < cant; i++){
            if(i == opcion)
                DibujarTextoCentrado(opciones[i], Y, W,0);
            else
                DibujarTextoCentrado(opciones[i], Y, 4,0);
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
    while(1)
    {

        char *opciones[] = {"REANUDAR", "SALIR"};
        ImprimirMenu(opcion,opciones,2);


        gbt_procesar_entrada();
        CalcularOpcion(&tecla,&opcion,2);


        if(tecla == GBTK_ENTER)
            switch(opcion){
                case 0: return REANUDAR; break;
                case 1: return SALIR; break;
            }
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
