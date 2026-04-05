#ifndef __UTILES__
#define __UTILES__

#define MAX_PAREDES 600
#define MAX_CAMINO 600

#include "ascension_magios.h"


/*
 *  Recibe dos vectores de coordenadas y su tope. En el primer vector se almacenan las paredes y en el segundo el camino de ese nivel.
 *  Inicializa los vectores con las coordenadas de algún mapa para un nivel dado.
*/
void obtener_mapa(coordenada_t paredes[MAX_PAREDES], int* tope_paredes, coordenada_t camino[MAX_CAMINO], int* tope_camino, int nivel);

#endif