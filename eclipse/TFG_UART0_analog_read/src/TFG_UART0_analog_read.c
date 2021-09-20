/*
============================================================================
 Name        : TFG_UART0_analog_read.c
 Author      : álvaro
 Version     :
 Copyright   : Your copyright notice
 Description : Selección canal ADC a leer mediante UART0
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANALOG_CHANNELS_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

int analogRead (int channel);
int analog_value;
int channel;

int main(void) {

	printf("Comunicación mediante UART0 para seleccionar el canal analógico que se desea leer...\n");
	printf("Para finalizar el programa pulsar Ctrl+C\n");

	while(1){
		printf("Seleccionar el canal analógico a leer (2,3,5 ó 6)\n");
		scanf("%d", &channel);
		printf("Canal %d seleccionado.\n", channel);
		analog_value = analogRead (channel);
		printf("Lectura analógica: %d\n\n", analog_value);
	}
}

int analogRead (int channel) {
	int value;															 // almacena valor analogico
	FILE* ioval; 													 	 // puntero apertura archivo
	char analog_path[100]; 											 	 // contendra el path al archivo
	sprintf(analog_path, ANALOG_CHANNELS_PATH "%d" "_raw", channel); 	 // composicion del path del archivo del canal del ADC
	ioval = fopen(analog_path, "r"); 									 // apertura del archivo para lectura
	fscanf(ioval, "%d", &value);										 // contenido del puntero al entero
	fclose(ioval); 														 // cierre del archivo con el puntero
	return value;
}
