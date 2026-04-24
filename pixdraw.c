#include <stdio.h>
#include <stdlib.h>
#include "GBT/gbt.h"
#include "menu.h"


void DrawTile(uint16_t X_OR, uint16_t Y_OR, uint8_t COLOR_INDEX)
{

    uint16_t X_LIM, Y_LIM, px, py;

    X_LIM = X_OR + TILE_SIZE;
    Y_LIM = Y_OR + TILE_SIZE;
    for(py=Y_OR ; py < Y_LIM ;py++){
        for(px=X_OR ; px < X_LIM  ;px++){
            gbt_dibujar_pixel(px,py,COLOR_INDEX);
        }
    }

    gbt_volcar_backbuffer();

}

void DrawMatrix(uint8_t Matrix[15][15],uint16_t X_OR, uint16_t Y_OR)
{
    uint8_t f,c;
    for(f=0;f<15;f++){
        for(c=0;c<15;c++){
            if(Matrix[f][c]!=0){

                uint16_t X_POS = X_OR + c*TILE_SIZE, Y_POS = Y_OR + f*TILE_SIZE;
                DrawTile(X_POS,Y_POS,Matrix[f][c]);
            }
        }
    }
}
