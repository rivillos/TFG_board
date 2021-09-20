/*
 * main.c
 *
 *  Created on: 10 sept. 2020
 *      Author: alvaro
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>		//para la función open
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* MPU6050 registers addresses */

#define MPU6050_PWR_REG				0x6B
#define MPU6050_ACC_CONF_REG 		0x1C
#define MPU6050_GYRO_CONF_REG 		0x1B

#define MPU6050_ACC_X_HIGH_REG		0x3B
#define MPU6050_ACC_X_LOW_REG		0x3C
#define MPU6050_ACC_Y_HIGH_REG		0x3D
#define MPU6050_ACC_Y_LOW_REG		0x3E
#define MPU6050_ACC_Z_LOW_REG		0x3F
#define MPU6050_ACC_Z_HIGH_REG		0x40

#define MPU6050_GYRO_X_HIGH_REG		0x43
#define MPU6050_GYRO_X_LOW_REG		0x44
#define MPU6050_GYRO_Y_HIGH_REG		0x45
#define MPU6050_GYRO_Y_LOW_REG		0x46
#define MPU6050_GYRO_Z_HIGH_REG		0x47
#define MPU6050_GYRO_Z_LOW_REG		0x48

#define ACC_FS_SENSIVITY_0			16384
#define ACC_FS_SENSIVITY_1			8192
#define ACC_FS_SENSIVITY_2			4096
#define ACC_FS_SENSIVITY_3			2048

#define GYRO_FS_SENSIVITY_0			131
#define GYRO_FS_SENSIVITY_1			65.5
#define GYRO_FS_SENSIVITY_2			32.8
#define GYRO_FS_SENSIVITY_3			16.4

#define MPU6050_I2C_ADDRESS			0x68

#define I2C0_FILE 	"/dev/i2c-0"

int fd;

int MPU6050_write(uint8_t address, uint8_t data) {	// tipos de argumentos enteros sin signos de 8 bits (0x00)
	int ret;
	char buf[2];
	buf[0] = address;
	buf[1] = data;
	if ((ret = write(fd,buf,2)) <= 0) {
		perror("Error de escritura\n");
		return -1;
	}
	return 0;
}

int MPU6050_read(uint8_t address, char *pBuffer, uint32_t length) {
	int ret;
	char buf[2];
	buf[0] = address;
	if ((ret = write(fd,buf,1)) <= 0) {
		perror("Error de escritura\n");
		return -1;
	}
	if ((ret = read(fd,pBuffer, length)) <= 0) {
		perror("Error de lectura\n");
	}
	return 0;
}

void init_MPU6050 (void) {
	//Sacar al MPU6050 del modo sleep (registro 0x6B a 0x00)
	MPU6050_write(MPU6050_PWR_REG, 0x00);
	usleep(500);
	//Configurar el acelerómetro y el giróscopo con máxima precisión (0x1C a 0x18 y el 0x1B a 0x18)
	MPU6050_write(MPU6050_ACC_CONF_REG, 0x18);
	usleep(500);
	MPU6050_write(MPU6050_GYRO_CONF_REG, 0x18);
	usleep(500);
}

void MPU6050_read_acc(short *pBuffer) {
	// cada eje está compuesto por 2 bytes. Son necesarios 6 bytes
	char acc_buffer[6];
	//lectura de los 6 registros de los 3 ejes del acelerómetro
	MPU6050_read(MPU6050_ACC_X_HIGH_REG,acc_buffer,6);

	pBuffer[0] = (int) ((acc_buffer[0] << 8) | acc_buffer[1] );
	pBuffer[1] = (int) ((acc_buffer[2] << 8) | acc_buffer[3] );
	pBuffer[2] = (int) ((acc_buffer[4] << 8) | acc_buffer[5] );
}

void MPU6050_read_gyro(short *pBuffer) {
	// cada eje está compuesto por 2 bytes. Son necesarios 6 bytes
	char gyro_buffer[6];
	//lectura de los 6 registros de los 3 ejes del acelerómetro
	MPU6050_read(MPU6050_GYRO_X_HIGH_REG,gyro_buffer,6);

	pBuffer[0] = (int) ((gyro_buffer[0] << 8) | gyro_buffer[1] );
	pBuffer[1] = (int) ((gyro_buffer[2] << 8) | gyro_buffer[3] );
	pBuffer[2] = (int) ((gyro_buffer[4] << 8) | gyro_buffer[5] );
}


int main(void) {

	short acc_value[3], gyro_value[3];	// 16 bits para guardar el valor de cada uno de los registros de 16 bits
	double acc_x,acc_y, acc_z, gyro_x, gyro_y, gyro_z;

	/* apertura del archivo del dispositivo i2c o */
	if ((fd = open(I2C0_FILE, O_RDWR)) < 0) {
		perror ("Error al abrir archivo I2C0\n");
		return -1;
	}

	/* insertar la dirección I2C utilizando la función ioctl con el comando I2C_SLAVE */
	if (ioctl(fd,I2C_SLAVE, MPU6050_I2C_ADDRESS) < 0) {
		perror ("Error al configurar la dirección I2C0\n");
		close(fd);
		return -1;
	}

	/* inicialización y configuración del MPU6050 */
	init_MPU6050();

	while (1) {
		MPU6050_read_acc(acc_value);
		MPU6050_read_gyro(gyro_value);

		acc_x = acc_value[0]/ACC_FS_SENSIVITY_3;
		acc_y = acc_value[1]/ACC_FS_SENSIVITY_3;
		acc_z = acc_value[2]/ACC_FS_SENSIVITY_3;

		gyro_x = gyro_value[0]/GYRO_FS_SENSIVITY_3;
		gyro_y = gyro_value[1]/GYRO_FS_SENSIVITY_3;
		gyro_z = gyro_value[2]/GYRO_FS_SENSIVITY_3;

		printf("Acelerómetro: x = %f, y = %f, z = %f\n", acc_x, acc_y, acc_z);
		printf("Giróscopo: x = %f, y = %f, z = %f\n", gyro_x, gyro_y, gyro_z);

		usleep(500000);
	}

}


