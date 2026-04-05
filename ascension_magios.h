#ifndef __ASCENSION_MAGIOS_H__
#define __ASCENSION_MAGIOS_H__

#include <stdbool.h>

#define MAX_FILAS 20
#define MAX_COLUMNAS 30
#define MAX_ELEMENTOS 40
#define MAX_PAREDES 600
#define MAX_CAMINO 600
#define MAX_NIVELES 3

typedef struct coordenada {
    int fil;
    int col;
} coordenada_t;

typedef struct personaje {
    coordenada_t posicion;
    bool recolecto_pergamino;
    int hechizos_reveladores;
    int vidas_restantes; 
    int antorchas;
    bool antorcha_encendida;
} personaje_t;

typedef struct objeto {
    char tipo;
    coordenada_t posicion;
} objeto_t;

typedef struct nivel {
    coordenada_t paredes[MAX_PAREDES];
    int tope_paredes;
    coordenada_t camino[MAX_CAMINO];
    int tope_camino;
    coordenada_t pergamino;
    objeto_t herramientas[MAX_ELEMENTOS];
    int tope_herramientas;
    objeto_t obstaculos[MAX_ELEMENTOS];
    int tope_obstaculos;
} nivel_t;

typedef struct juego {
    personaje_t homero;
    nivel_t niveles[MAX_NIVELES];
    int tope_niveles;
    int nivel_actual;
    bool camino_visible;
} juego_t;

/*
 * Pre condiciones: -
 * Post condiciones: Inicializará el juego, cargando toda la información inicial de Homero y de los 3 niveles.
 */
void inicializar_juego(juego_t *juego); //listo


/*
 * Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego` y el nivel actual del juego tiene que ser 1 o 2.
 * Post condiciones: Actualizará el nivel actual el juego y el personaje para comenzar el siguiente nivel.
 */
void cambiar_nivel(juego_t* juego);

/*
 * Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego` y la acción
 * debe ser válida.
 * Post condiciones: Realizará la acción recibida por parámetro actualizando el juego.
 */
void realizar_jugada(juego_t *juego, char movimiento);

/*
 * Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego `.
 * Post condiciones: Imprime el juego por pantalla.
 */
void mostrar_juego(juego_t juego);

/*
 * 'nivel' tiene todas sus estructuras válidas.
 * El nivel se dará por ganado cuando el pergamino esté en el altar
 * Devolverá:
 * >  0 si el estado es jugando.
 * >  1 si el estado es ganado.
 */
int estado_nivel(nivel_t nivel, personaje_t homero);

/*
 * Pre condiciones: El juego deberá estar inicializado previamente con `inicializar_juego `
 * Post condiciones: Devuelve:
 * --> 1 si es ganado
 * --> -1 si es perdido
 * --> 0 si se sigue jugando
 * El juego se dará por ganado cuando Homero lleve todos los pergaminos a sus altares correspondientes.
 * Se dará por perdido si se le termina la vida antes de llevar todos los pergaminos a sus altares.
 */
int estado_juego(juego_t juego);


#endif // __ASCENSION_MAGIOS_H__