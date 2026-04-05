#include "ascension_magios.h"
#include "utiles.h" //mapa
#include <stdio.h>
#include <stdlib.h> // rand

//AUXILIAR
int distancia_manhattan(coordenada_t p1, coordenada_t p2) {
    return abs(p1.fil - p2.fil) + abs(p1.col - p2.col);
}
bool es_posicion_valida(nivel_t nivel, coordenada_t pos) {
    // 1. ¿Es una pared?
    for (int i = 0; i < nivel.tope_paredes; i++) {
        if (nivel.paredes[i].fil == pos.fil && nivel.paredes[i].col == pos.col) {
            return false;
        }
    }
    // Podés agregar aquí chequeos para que no pise el Altar o la Runa si querés ser más estricto
    return true;
}
void inicializar_herramientas(nivel_t *nivel) {
    nivel->tope_herramientas = 0;
    while (nivel->tope_herramientas < 5) {
        coordenada_t pos_rand;
        pos_rand.fil = rand() % MAX_FILAS;
        pos_rand.col = rand() % MAX_COLUMNAS;

        if (es_posicion_valida(*nivel, pos_rand)) {
            nivel->herramientas[nivel->tope_herramientas].posicion = pos_rand;
            nivel->herramientas[nivel->tope_herramientas].tipo = 'T'; // 'T' de Tótem
            nivel->tope_herramientas++;
        }
    }
}
void inicializar_obstaculos(nivel_t *nivel) {
    nivel->tope_obstaculos = 0;

    // A. PIEDRAS (10)
    while (nivel->tope_obstaculos < 10) {
        coordenada_t pos_rand = {rand() % MAX_FILAS, rand() % MAX_COLUMNAS};

        if (es_posicion_valida(*nivel, pos_rand)) {
            nivel->obstaculos[nivel->tope_obstaculos].posicion = pos_rand;
            nivel->obstaculos[nivel->tope_obstaculos].tipo = 'O'; // 'O' de Obstáculo/Piedra
            nivel->tope_obstaculos++;
        }
    }

    // B. CATAPULTA (1) - Fuera del camino
    bool catapulta_puesta = false;
    while (!catapulta_puesta) {
        coordenada_t pos_rand = {rand() % MAX_FILAS, rand() % MAX_COLUMNAS};
        
        // Verificar que no sea pared
        if (es_posicion_valida(*nivel, pos_rand)) {
            // Verificar que NO sea camino
            bool es_camino = false;
            for(int i = 0; i < nivel->tope_camino; i++) {
                if(nivel->camino[i].fil == pos_rand.fil && nivel->camino[i].col == pos_rand.col) es_camino = true;
            }

            if (!es_camino) {
                nivel->obstaculos[nivel->tope_obstaculos].posicion = pos_rand;
                nivel->obstaculos[nivel->tope_obstaculos].tipo = 'C'; // 'C' de Catapulta
                nivel->tope_obstaculos++;
                catapulta_puesta = true;
            }
        }
    }
}

//BIBLIOTECA DE FUNCIONES PARA EL JUEGO
void inicializar_juego(juego_t *juego){
    for (int i = 0; i < MAX_NIVELES; i++) {
        obtener_mapa(juego->niveles[i].paredes, &juego->niveles[i].tope_paredes, juego->niveles[i].camino, &juego->niveles[i].tope_camino, i + 1);
        // 2. Pergamino (en el camino)
        int pos_media = rand() % juego->niveles[i].tope_camino;
        juego->niveles[i].pergamino = juego->niveles[i].camino[pos_media];

        // 3. Herramientas y Obstáculos
        inicializar_herramientas(&(juego->niveles[i]));
        inicializar_obstaculos(&(juego->niveles[i]));
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
    system("clear");
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
    
   bool en_runa = (juego.homero.posicion.fil == nivel_actual.camino[0].fil && 
                    juego.homero.posicion.col == nivel_actual.camino[0].col);

    // CAMINO
    for (int i = 0; i < nivel_actual.tope_camino; i++) {
        coordenada_t pos_camino = nivel_actual.camino[i];
        
        // El camino se ve si: Hechizo activo OR está en la Runa OR Antorcha cerca
        if (juego.camino_visible || en_runa ||
           (juego.homero.antorcha_encendida && distancia_manhattan(juego.homero.posicion, pos_camino) <= 3)) {
            mapa_nivel[pos_camino.fil][pos_camino.col] = '.';
        }
    }
    // PERGAMINO Y ALTAR

        // Dibujar Herramientas
    for (int i = 0; i < nivel_actual.tope_herramientas; i++) {
        mapa_nivel[nivel_actual.herramientas[i].posicion.fil][nivel_actual.herramientas[i].posicion.col] = 'T';
    }

    // Dibujar Obstáculos
    for (int i = 0; i < nivel_actual.tope_obstaculos; i++) {
        char tipo = nivel_actual.obstaculos[i].tipo;
        mapa_nivel[nivel_actual.obstaculos[i].posicion.fil][nivel_actual.obstaculos[i].posicion.col] = tipo;
    }

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
        juego->homero.antorcha_encendida = false;
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
    if (proxima_pos.fil < 0 || proxima_pos.fil >= MAX_FILAS || 
        proxima_pos.col < 0 || proxima_pos.col >= MAX_COLUMNAS) {
        return; // Movimiento inválido, no hace nada.
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
        juego->camino_visible = true;
        printf("¡Has recolectado el pergamino!\n");
    }
    //totem?
    for (int i = 0; i < nivel_actual->tope_herramientas; i++) {
    if (juego->homero.posicion.fil == nivel_actual->herramientas[i].posicion.fil &&
        juego->homero.posicion.col == nivel_actual->herramientas[i].posicion.col) {
        
        if (nivel_actual->herramientas[i].tipo == 'T') {
            juego->homero.vidas_restantes++;
            printf("¡Tótem encontrado! +1 Vida.\n");
            // Eliminar el tótem para que no dé vidas infinitas
            nivel_actual->herramientas[i] = nivel_actual->herramientas[nivel_actual->tope_herramientas - 1];
            nivel_actual->tope_herramientas--;
            i--;
        }
    }
}
for (int i = 0; i < nivel_actual->tope_obstaculos; i++) {
        if (juego->homero.posicion.fil == nivel_actual->obstaculos[i].posicion.fil &&
            juego->homero.posicion.col == nivel_actual->obstaculos[i].posicion.col) {
            
            if (nivel_actual->obstaculos[i].tipo == 'O') {
                printf("¡Piedra del castigo! El pergamino cambió de lugar.\n");
                int nueva_pos = rand() % nivel_actual->tope_camino;
                nivel_actual->pergamino = nivel_actual->camino[nueva_pos];
                juego->homero.recolecto_pergamino = false; 
                
                // Eliminar la piedra
                nivel_actual->obstaculos[i] = nivel_actual->obstaculos[nivel_actual->tope_obstaculos - 1];
                nivel_actual->tope_obstaculos--;
                i--; // Revisar la nueva piedra que quedó en esta posición
            }
            else if (nivel_actual->obstaculos[i].tipo == 'C') {
                printf("¡Catapulta! Has sido lanzado al inicio del nivel.\n");
                juego->homero.posicion = nivel_actual->camino[0];
            }
        }
    }
    
    

    // 5. Lógica de Altar y cambio de nivel
    if (juego->homero.posicion.fil == altar.fil && juego->homero.posicion.col == altar.col) {
        if (estado_nivel(*nivel_actual, juego->homero) == 1) {
            cambiar_nivel(juego);
            printf("¡Nivel completado!\n");
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

int estado_nivel(nivel_t nivel, personaje_t homero) {
    coordenada_t altar = nivel.camino[nivel.tope_camino - 1];
    
    if (homero.posicion.fil == altar.fil && 
        homero.posicion.col == altar.col && 
        homero.recolecto_pergamino) {
        return 1; // Nivel ganado
    }
    return 0; // Sigue jugando
}

void cambiar_nivel(juego_t* juego) {
    juego->nivel_actual++;
    if (juego->nivel_actual < juego->tope_niveles) {
        // Resetear a Homero para el nuevo nivel
        juego->homero.posicion = juego->niveles[juego->nivel_actual].camino[0];
        juego->homero.recolecto_pergamino = false;
        // Apagamos ayudas por las dudas
        juego->camino_visible = false;
        juego->homero.antorcha_encendida = false;
    }
}