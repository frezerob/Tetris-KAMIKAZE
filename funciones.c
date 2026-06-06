#include "funciones.h"
#include "core.h"
#include "config.h"
uint8_t PiezaAnterior;

/**
 * Inicializa la semilla del generador de números pseudoaleatorios utilizando
 * el tiempo actual del sistema para asegurar secuencias diferentes en cada ejecución.
 */
void semilla()
{
    srand(time(NULL));
}

/**
 * Genera un índice de pieza aleatorio asegurando que no se repita de forma
 * consecutiva con la pieza anterior. Adapta el pool de piezas disponibles (11 o 7)
 * según si el juego se encuentra en modo DELUXE o estándar.
 *
 * Retorna: El identificador numérico de la nueva pieza generada.
 */
uint8_t generarPiezaAleatoria() {
    uint8_t Aux;

    uint8_t cantidadPiezas = (config.MODO == DELUXE) ? 11 : 7;

    do{
        Aux = (rand() % cantidadPiezas);
    }
    while(Aux == PiezaAnterior);
    PiezaAnterior = Aux;
    return PiezaAnterior;
}

uint8_t PIEZA_I[4][16] = {
    {TR,TR,TR,TR, I,I,I,I, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,TR,I,TR, TR,TR,I,TR, TR,TR,I,TR, TR,TR,I,TR},
    {TR,TR,TR,TR, TR,TR,TR,TR, I,I,I,I, TR,TR,TR,TR},
    {TR,I,TR,TR, TR,I,TR,TR, TR,I,TR,TR, TR,I,TR,TR}
};

uint8_t PIEZA_J[4][16] = {
    {J,TR,TR,TR, J,J,J,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,J,J,TR, TR,J,TR,TR, TR,J,TR,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, J,J,J,TR, TR,TR,J,TR, TR,TR,TR,TR},
    {TR,J,TR,TR, TR,J,TR,TR, J,J,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_L[4][16] = {
    {TR,TR,L,TR, L,L,L,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,L,TR,TR, TR,L,TR,TR, TR,L,L,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, L,L,L,TR, L,TR,TR,TR, TR,TR,TR,TR},
    {L,L,TR,TR, TR,L,TR,TR, TR,L,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_O[4][16] = {
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,O,O,TR, TR,O,O,TR, TR,TR,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_S[4][16] = {
    {TR,S,S,TR, S,S,TR,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,S,TR,TR, TR,S,S,TR, TR,TR,S,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, TR,S,S,TR, S,S,TR,TR, TR,TR,TR,TR},
    {S,TR,TR,TR, S,S,TR,TR, TR,S,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_T[4][16] = {
    {TR,T,TR,TR, T,T,T,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,T,TR,TR, TR,T,T,TR, TR,T,TR,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, T,T,T,TR, TR,T,TR,TR, TR,TR,TR,TR},
    {TR,T,TR,TR, T,T,TR,TR, TR,T,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_Z[4][16] = {
    {Z,Z,TR,TR, TR,Z,Z,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,TR,Z,TR, TR,Z,Z,TR, TR,Z,TR,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, Z,Z,TR,TR, TR,Z,Z,TR, TR,TR,TR,TR},
    {TR,Z,TR,TR, Z,Z,TR,TR, Z,TR,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_X[4][16] = {
    {TR,Z,TR,TR, TR,TR,TR,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,Z,TR,TR, TR,TR,TR,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,Z,TR,TR, TR,TR,TR,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,Z,TR,TR, TR,TR,TR,TR, TR,TR,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_P[4][16] = {
    {Z,Z,TR,TR, Z,Z,TR,TR, Z,TR,TR,TR, TR,TR,TR,TR},
    {TR,Z,Z,Z, TR,TR,Z,Z, TR,TR,TR,TR, TR,TR,TR,TR},
    {TR,TR,TR,TR, TR,TR,TR,Z, TR,TR,Z,Z, TR,TR,Z,Z},
    {TR,Z,TR,TR, Z,Z,TR,TR, Z,Z,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_C[4][16] = {
    {Z,Z,TR,TR, Z,TR,TR,TR, Z,Z,TR,TR, TR,TR,TR,TR},
    {Z,Z,Z,TR, Z,TR,Z,TR, TR,TR,TR,TR, TR,TR,TR,TR},
    {Z,Z,TR,TR, TR,Z,TR,TR, Z,Z,TR,TR, TR,TR,TR,TR},
    {Z,TR,Z,TR, Z,Z,Z,TR, TR,TR,TR,TR, TR,TR,TR,TR}
};

uint8_t PIEZA_H[4][16] = {
    {O,TR,O,TR, O,O,O,TR, O,TR,O,TR, TR,TR,TR,TR},
    {O,O,O,TR, TR,O,TR,TR, O,O,O,TR, TR,TR,TR,TR},
    {O,TR,O,TR, O,O,O,TR, O,TR,O,TR, TR,TR,TR,TR},
    {O,O,O,TR, TR,O,TR,TR, O,O,O,TR, TR,TR,TR,TR},
};

uint8_t (*FORMAS[11])[16] = {
    PIEZA_I, PIEZA_J, PIEZA_L, PIEZA_O, PIEZA_S, PIEZA_T, PIEZA_Z, PIEZA_X, PIEZA_C, PIEZA_P, PIEZA_H
};

/**
 * Configura los atributos iniciales de una estructura PiezaActiva (forma, rotación,
 * posición vertical). Calcula su coordenada horizontal inicial en base al historial;
 * si opera en modo DELUXE aplica comportamiento toroidal (bucle en los bordes), de lo
 * contrario valida los límites físicos del tablero para evitar desbordamientos laterales.
 *
 * pieza: Puntero a la estructura de la pieza actual que se va a inicializar.
 * tipoSeleccionado: Índice numérico de la pieza dentro del arreglo global 'FORMAS'.
 * m: Puntero a la estructura de la matriz de juego.
 */
void tipoPieza(PiezaActiva* pieza, uint8_t tipoSeleccionado, matrix* m) {
    pieza->forma = FORMAS[tipoSeleccionado];
    pieza->rotacion = 0;
    pieza->posY = -1;

    pieza->posX = config.ultimaPosX;

    if (config.MODO == DELUXE) {
        pieza->posX = (pieza->posX % config.COL_TABLERO + config.COL_TABLERO) % config.COL_TABLERO;
    }
    else {
        uint16_t max_ancho_ocupado = 0;
        for(uint8_t i = 0; i < ORDEN; i++) {
            for(uint8_t j = 0; j < ORDEN; j++) {
                if(pieza->forma[0][i * ORDEN + j] != TR) {
                    if(j > max_ancho_ocupado) {
                        max_ancho_ocupado = j;
                    }
                }
            }
        }

        if(pieza->posX + max_ancho_ocupado >= config.COL_TABLERO) {
            pieza->posX = config.COL_TABLERO - (max_ancho_ocupado + 1);
        }
        if(pieza->posX < 0) {
            pieza->posX = 0;
        }
    }
}

/**
 * Decrementa la posición vertical de la pieza en una unidad, simulando un
 * desplazamiento hacia la parte superior de la pantalla.
 *
 * p: Puntero a la estructura de la pieza activa.
 */
void PiezaMoverArriba(PiezaActiva* p)
{
    p->posY--;
}

/**
 * Incrementa la posición vertical de la pieza en una unidad, simulando un
 * desplazamiento descendente en el tablero.
 *
 * p: Puntero a la estructura de la pieza activa.
 */
void PiezaMoverAbajo(PiezaActiva* p)
{
    p->posY++;
}

/**
 * Decrementa la posición horizontal de la pieza en una unidad, desplazándola
 * hacia la izquierda del tablero.
 *
 * p: Puntero a la estructura de la pieza activa.
 */
void PiezaMoverIzq(PiezaActiva* p)
{
    p->posX--;
}

/**
 * Incrementa la posición horizontal de la pieza en una unidad, desplazándola
 * hacia la derecha del tablero.
 *
 * p: Puntero a la estructura de la pieza activa.
 */
void PiezaMoverDer(PiezaActiva* p)
{
    p->posX++;
}

/**
 * Cambia el estado de rotación de la pieza activa avanzando un paso en sentido
 * horario dentro de sus 4 fases posibles (0 a 3).
 *
 * p: Puntero a la estructura de la pieza activa.
 */
void PiezaRotarDerecha(PiezaActiva* p)
{
    p->rotacion = (p->rotacion + 1) % 4;
}

/**
 * Verifica si la pieza colisiona con los bordes del escenario, con el fondo (piso)
 * o con bloques estáticos ya consolidados en la matriz de juego. Contempla el
 * comportamiento envolvente (toroidal) de los límites horizontales en el modo DELUXE.
 *
 * p: Puntero a la estructura de la pieza activa en su estado/posición a evaluar.
 * m: Puntero a la matriz donde se registran los bloques fijados del juego.
 *
 * Retorna: 1 si se detecta alguna colisión, 0 si la posición es completamente válida.
 */
int8_t PiezaDetectarColision(PiezaActiva* p, matrix* m)
{
    for(uint8_t i = 0; i < ORDEN; i++){
        for(uint8_t j = 0; j < ORDEN; j++){
            if(p->forma[p->rotacion][i*ORDEN+j] == TR) continue;

            int fila = p->posY + i;
            int col  = p->posX + j;

            if(fila >= config.FIL_TABLERO) return 1; // piso

            if(config.MODO == DELUXE){
                col = (col % config.COL_TABLERO + config.COL_TABLERO) % config.COL_TABLERO;
                if(fila >= 0 && m->mat[fila][col] != N) return 1;
            }
            else{
                if(col < 0 || col >= config.COL_TABLERO) return 1;
                if(fila >= 0 && m->mat[fila][col] != N) return 1;
            }

        }
    }
    return 0;
}

/**
 * Cambia el estado de rotación de la pieza activa retrocediendo un paso en sentido
 * antihorario dentro de sus 4 fases posibles (0 a 3).
 *
 * p: Puntero a la estructura de la pieza activa.
 */
void PiezaRotarIzquierda(PiezaActiva* p)
{
    p->rotacion = (p->rotacion + 3) % 4;
}

/**
 * Reduce de manera porcentual el tiempo de refresco/caída del bucle (haciéndolo
 * más rápido) en función de la cantidad de piezas colocadas, subdividiendo
 * la progresión cada 10 bloques (niveles implícitos).
 *
 * velInicial: Velocidad o retardo base expresado en milisegundos.
 * piezas: Contador total de piezas colocadas hasta el momento.
 *
 * Retorna: El nuevo valor numérico de delay dinámico calculado.
 */
int RecalcularVelocidad(int velInicial, int piezas)
{
    int niveles = piezas / 10;
    int vel = velInicial;
    for(int i = 0; i < niveles; i++)
        vel = (vel * 97) / 100;
    return vel;
}

/**
 * Devuelve el tiempo de retardo por defecto (en milisegundos) asociado a los
 * tres modos de juego predefinidos del sistema.
 *
 * dificultad: Nivel de dificultad (0 para FÁCIL, 1 para NORMAL, 2 para DIFÍCIL).
 *
 * Retorna: Tiempo de espera en milisegundos (1000, 500 o 200 respectivamente).
 */
uint16_t VelocidadSegunDificultad(int dificultad)
{
    switch(dificultad){
        case 0: return 1000;
        case 1: return 500;
        case 2: return 200;
        default: return 1000;
    }
}

/**
 * Calcula el coeficiente multiplicador aplicable a la puntuación basándose en
 * la tasa de frames/velocidad de caída actual. Cuanto menor sea el delay (más rápido),
 * mayor será el puntaje otorgado.
 */
int MultiplicadorPuntos(int velActual)
{
    return 1000 / velActual;
}

/**
 * Compara los caracteres de una cadena de texto dada con el string literal "GOD"
 * bajo un límite definido por la constante MAX_NOMBRE para verificar coincidencia exacta.
 *
 * nombre: Puntero a la cadena de caracteres que se desea comprobar.
 *
 * Retorna: 1 si el nombre coincide exactamente con "GOD", 0 en caso contrario.
 */
uint8_t NombreEsGod(char *nombre)
{
    char *GOD = "GOD";
    for(uint8_t c=0;c<MAX_NOMBRE;c++){
        if(nombre[c] != GOD[c])
            return 0;
    }
    return 1;
}
