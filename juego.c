#include <stdio.h>
#include <stdlib.h> 
#include "ascension_magios.h"
#include "utiles.h"

char solicitar_movimiento() {
    char movimiento = '\0';
    printf("Ingrese el movimiento o herramienta.. \n-W- (ADELANTE)\n -A- (ATRAS)\n -S- (IZQUIERDA)\n -D- (DERECHA)\n -H-(HECHIZO REVELADOR)\n -L- (ANTORCHAS)\n OPCION: ");
    scanf(" %c", &movimiento);
    return movimiento;
}

int main(){
    juego_t juego;
    char movimiento;

    inicializar_juego(&juego);
    
    while (estado_juego(juego) == 0) {
        mostrar_juego(juego);
        movimiento = solicitar_movimiento();
        realizar_jugada(&juego, movimiento);
    }
    mostrar_juego(juego); // perdido o ganado
    
    int resultado = estado_juego(juego);
    if (resultado == 1) {
        printf("\n¡FELICITACIONES! Has ascendido a MAGIO.\n");
    } else {
        printf("\nGAME OVER...\n");
        printf("Te espera la PIEDRA DEL CASTIGO!\n");
    }
}