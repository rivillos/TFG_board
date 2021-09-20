/*
 ============================================================================
 Name        : TFG_GPIOs_control.c
 Author      : álvaro
 Version     :
 Copyright   : Your copyright notice
 Description : Control de un led mediante un pulsador
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void configGPIO (char pin_kernel[], char direction[]);
void writeGPIO (char pin_kernel[], char value[]);
int readGPIO (char pin_kernel[]);

int main(void) {

	configGPIO("50", "out");					// configuracion pin(kernel) 50 como salida
	configGPIO("57", "in");						// configuracion pin(kernel) 57 como entrada
	printf("GPIOs control...\n");
	printf("Pulsar BTN2 para encender LED\n");
	printf("Liberar BTN2 para apagar LED\n\n");
	printf("Para finalizar el programa pulsar Ctrl+C\n");

	while(1){
		if (readGPIO("57"))						// lectura pin(kernel) 57
			writeGPIO("50", "0");				// escritura nivel bajo pin(kernel) 50
		else
			writeGPIO("50", "1");				// escritura nivel alto pin(kernel) 50
	}
}

void configGPIO (char pin_kernel[], char direction[]) {
	FILE *io, *iodir;															// punteros para apertura de archivos
	io = fopen("/sys/class/gpio/export", "w+"); 								// apertura del archivo export para exportar el gpio
	fprintf(io, "%s", pin_kernel); 												// escritura en el archivo export del nuevo gpio para exportarlo
	fclose(io); 																// cierre del archivo con el puntero
	char gpio_path[100];														// contendra el path al archivo
	sprintf(gpio_path, "/sys/class/gpio/gpio" "%s" "/direction", pin_kernel);   // composicion del path del archivo direction del gpio
	iodir = fopen(gpio_path, "w+"); 											// apertura del archivo direction para escritura
	fprintf(iodir, "%s", direction); 											// escritura en el archivo direction (in/out)
	fclose(iodir); 																// cierre del archivo con el puntero
}

void writeGPIO (char pin_kernel[], char value[]) {
	FILE* ioval; 																// puntero apertura archivo
	char gpio_path[100]; 														// contendra el path al archivo
	sprintf(gpio_path, "/sys/class/gpio/gpio" "%s" "/value", pin_kernel);		// composicion del path del archivo value del gpio
	ioval = fopen(gpio_path, "w+"); 											// apertura del archivo value para escritura
	fprintf(ioval, "%s", value); 												// escritura en el archivo value
	fclose(ioval);															    // cierre del archivo con el puntero
}

int readGPIO (char pin_kernel[]) {
	int value;																	// almacena estado pin
	FILE* ioval; 																// puntero apertura archivo
	char gpio_path[100]; 														// contendra el path al archivo
	sprintf(gpio_path, "/sys/class/gpio/gpio" "%s" "/value", pin_kernel); 		// composicion del path del archivo value del gpio
	ioval = fopen(gpio_path, "r"); 												// apertura del archivo value para lectura
	fscanf(ioval, "%d", &value);												// contenido del puntenro al entero
	fclose(ioval); 																// cierre del archivo con el puntero
	return value;
}
