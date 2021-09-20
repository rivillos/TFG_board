/*
============================================================================
 Name        : TFG_analog_read.c
 Author      : álvaro
 Version     :
 Copyright   : Your copyright notice
 Description : Lectura analógica
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANALOG_CHANNELS_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

int analogRead (int channel);
int analog_value;

int main(void) {

	printf("Lectura analógica...\n");
	printf("Para finalizar el programa pulsar Ctrl+C\n");
	usleep(3000000);

	while(1){
		analog_value = analogRead (2);					// Lectura del canal 2
		printf("Lectura analógica: %d\n", analog_value);
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
