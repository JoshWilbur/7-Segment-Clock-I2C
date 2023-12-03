#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <time.h>

// Global variables
static int fd, result;
static char i2c_device[]="/dev/i2c-1";
static unsigned char buffer[17];
time_t current_time;

int ht16k33_setup(){
	// Open i2c device driver
	fd = open("/dev/i2c-1", O_RDWR);
	// Error check to make sure file is open
	if(fd < 0){
		perror("fd error: ");
		exit(1);
	}

	// Set slave address
	result=ioctl(fd, I2C_SLAVE, 0x70);
	// Error check by ensuring result isn't negative
	if(result < 0){
                perror("Slave address error: ");
		exit(1);
	}
	        printf("Slave address bytes written: %d\n", result);

	// Turn on oscillator
	buffer[0]=(0x2<<4)|(0x1);
	result=write(fd, buffer, 1);
	// Error checking
        if(result < 0){
                perror("Oscillator error: ");
                exit(1);
        }
	printf("Oscillator bytes written: %d\n", result);

	// Turn on Display, No Blink
	buffer[0] = 0;
	buffer[0]=(0x8<<4)|(0x1);
        result=write(fd, buffer, 1);
	// Error checking
        if(result < 0){
                perror("Display register error: ");
                exit(1);
        }
	printf("Display bytes written: %d\n", result);

	// Set Brightness
        buffer[0] = 0;
        buffer[0]=(0xE0)|(0x0C); // Set brightness to 13
        result=write(fd, buffer, 1);
	// Error checking
        if(result < 0){
                perror("Dimming error: ");
                exit(1);
        }
        printf("Brightness bytes written: %d\n", result);

	return 0;
}

int main(){
	// Load current time into struct
	time(&current_time);
	struct tm *now = localtime(&current_time);
        ht16k33_setup();
	int sec, min_1s, min_10s, hour_1s, hour_10s = 0;
	while(1){
		sec = now->tm_sec;
        	min = now->tm_min;
        	hour = now->tm_hour;
		buffer[0] = 0;
        	buffer[1] = 0b01000000;
       		buffer[3] = display_number(2) | 0b10000000;
        	buffer[7] = display_number(2);
        	buffer[9] = display_number(9);
        	result = write(fd, buffer, 17);
        	printf("Write bytes written: %d\n", result);
	}
}
