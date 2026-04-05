#include "ascension_magios.h"
#include "utiles.h" //mapa
#include <stdio.h>

//AUXILIAR
int distancia_manhattan(coordenada_t p1, coordenada_t p2) {
    return abs(p1.fil - p2.fil) + abs(p1.col - p2.col);
}


//BIBLIOTECA DE FUNCIONES PARA EL JUEGO
void inicializar_juego(juego_t *juego){
    for (int i = 0; i < MAX_NIVELES; i++) {
        obtener_mapa(juego->niveles[i].paredes, &juego->niveles[i].tope_paredes, juego->niveles[i].camino, &juego->niveles[i].tope_camino, i + 1);
        juego->niveles[i].tope_herramientas = 0;
        juego->niveles[i].tope_obstaculos = 0;
        int pos_media = juego->niveles[i].tope_camino / 2;
        juego->niveles[i].pergamino = juego->niveles[i].camino[pos_media];
    }
    juego->homero.posicion = juego->niveles[0].camino[0];
    juego->homero.recolecto_pergamino = false;
    juego->homero.hechizos_reveladores = 5;
    juego->homero.vidas_restantes = 5; 
    juego->homero.antorchas = 5;
    juego->homero.antorcha_encendida = false;
    juego->tope_niveles = MAX_NIVELES;
    juego->nivel_actual = 0;
    juego->camino_visible = false;

}

void mostrar_juego(juego_t juego) {
    //CCODIGO CLEAR
    char mapa_nivel[MAX_FILAS][MAX_COLUMNAS];
    for (int i = 0; i < MAX_FILAS; i++) {
        for (int j = 0; j < MAX_COLUMNAS; j++) {
            mapa_nivel[i][j] = ' ';
        }
    }
    nivel_t nivel_actual = juego.niveles[juego.nivel_actual];

    // PAREDES
    for (int i = 0; i < nivel_actual.tope_paredes; i++) {
        coordenada_t pared = nivel_actual.paredes[i];
        mapa_nivel[pared.fil][pared.col] = 'X';
    }
    
    // CAMINO
    for (int i = 0; i < nivel_actual.tope_camino; i++) {
        coordenada_t pos_camino = nivel_actual.camino[i];
        
    
        // Mostrar el camino solo si es visible o si la antorcha está encendida y el jugador está cerca del camino
        if (juego.camino_visible || 
        (juego.homero.antorcha_encendida && distancia_manhattan(juego.homero.posicion, pos_camino) <= 3)) {
            mapa_nivel[pos_camino.fil][pos_camino.col] = '.';
        }
    }
    // PERGAMINO Y ALTAR

    if (!juego.homero.recolecto_pergamino) {
        coordenada_t pergamino = nivel_actual.pergamino;
        mapa_nivel[pergamino.fil][pergamino.col] = 'P';
    }

    coordenada_t altar = nivel_actual.camino[nivel_actual.tope_camino - 1];
    mapa_nivel[altar.fil][altar.col] = 'A';

    // HOMERO
    mapa_nivel[juego.homero.posicion.fil][juego.homero.posicion.col] = 'H';

    //UI
    printf("\n--- NIVEL %i ---\n", juego.nivel_actual + 1);
    printf("Vidas: %i | Hechizos: %i | Antorchas: %i\n", 
           juego.homero.vidas_restantes, juego.homero.hechizos_reveladores, juego.homero.antorchas);
    
    for (int f = 0; f < MAX_FILAS; f++) {
        for (int c = 0; c < MAX_COLUMNAS; c++) {
            printf("%c", mapa_nivel[f][c]);
        }
        printf("\n");
    }

}

void realizar_jugada(juego_t *juego, char movimiento) {
    nivel_t *nivel_actual = &(juego->niveles[juego->nivel_actual]);
    coordenada_t proxima_pos = juego->homero.posicion;
    coordenada_t altar = nivel_actual->camino[nivel_actual->tope_camino - 1];
    bool es_pared = false;

    if (movimiento == 'W' || movimiento == 'S' || movimiento == 'A' || movimiento == 'D') {
        juego->camino_visible = false;
    }
    if (movimiento == 'W') {
        proxima_pos.fil--;
    } 
    else if (movimiento == 'S') {
        proxima_pos.fil++;
    } 
    else if (movimiento == 'A') {
        proxima_pos.col--;
    } 
    else if (movimiento == 'D') {
        proxima_pos.col++;
    } 
    else if (movimiento == 'H' && juego->homero.hechizos_reveladores > 0) {
        juego->camino_visible = true;
        juego->homero.hechizos_reveladores--;
        return; 
    }
    else if (movimiento == 'L' && juego->homero.antorchas > 0) {
        juego->homero.antorcha_encendida = true;
        juego->homero.antorchas--;
        return; 
    }
    else {
        printf("Movimiento no válido. Intente nuevamente.\n");
        return;
    }

    // Verificar si la próxima posición es una pared, CHOCA?
    for (int i = 0; i < nivel_actual->tope_paredes; i++) {
        if (nivel_actual->paredes[i].fil == proxima_pos.fil && 
            nivel_actual->paredes[i].col == proxima_pos.col) {
            es_pared = true;
        }
    }
    // Después del for que verifica las paredes:
    if (es_pared) {
        return; // Si es pared, Homero no se mueve y termina la función
    }
    // Verificar si se sale del camino
    bool esta_en_camino = false;
        for (int i = 0; i < nivel_actual->tope_camino; i++) {
            if (nivel_actual->camino[i].fil == proxima_pos.fil && nivel_actual->camino[i].col == proxima_pos.col)
                esta_en_camino = true;
        }
        if (!esta_en_camino) {
            juego->homero.vidas_restantes--;
            printf("¡Te has salido del camino! -1 DE VIDA... VIDAS RESTANTES: %i\n", juego->homero.vidas_restantes);
            return;
        }
    
    // Si no es pared y no se sale del camino, mover a Homero
    juego->homero.posicion = proxima_pos;
        
    //pergamino?
    if (proxima_pos.fil == nivel_actual->pergamino.fil && 
        proxima_pos.col == nivel_actual->pergamino.col) {
        juego->homero.recolecto_pergamino = true;
        printf("¡Has recolectado el pergamino!\n");
    }
    
    

    // 5. Lógica de Altar y cambio de nivel
    if (juego->homero.posicion.fil == altar.fil && juego->homero.posicion.col == altar.col) {
        if (juego->homero.recolecto_pergamino) {
            juego->nivel_actual++;
            if (juego->nivel_actual < juego->tope_niveles) {
                juego->homero.posicion = juego->niveles[juego->nivel_actual].camino[0];
                juego->homero.recolecto_pergamino = false;
                printf("¡Nivel completado!\n");
            }
        } else {
            printf("Necesitas el pergamino para avanzar...\n");
        }
    }
}

int estado_juego(juego_t juego) {
    if (juego.homero.vidas_restantes <= 0) {
        printf("¡Has perdido todas tus vidas! Fin del juego.\n");
        return -1;
    } else if (juego.nivel_actual >= juego.tope_niveles) {
        printf("¡Felicidades, has completado todos los niveles!\n");
        return 1;
    } else {
        printf("El juego continúa. Nivel actual: %i\n", juego.nivel_actual + 1);
        return 0;
    }
}