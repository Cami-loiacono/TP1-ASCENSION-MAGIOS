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
    int vidas_iniciales = 5;
    juego_t juego;
    inicializar_juego(&juego);

}