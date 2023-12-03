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
struct tm *now;

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
        buffer[0]=(0xE0)|(0x01); // Set brightness to 1
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
        ht16k33_setup();
	buffer[0] = 0;
	int sec, min, min_1s, min_10s, hour24, hour12 = 0;
	while(1){
		// Update time;
		time(&current_time);
		now = localtime(&current_time);

		// Obtain second, minute an hour values from time struct
		sec = now->tm_sec;
        	min = now->tm_min;
        	hour24 = now->tm_hour;

		// Change hour value to 12 hr
		if (hour24 > 12) hour12 = hour24 - 12;

		// Obtain 10s and 1s values of minute
		min_10s = (min / 10);
		min_1s = min - (min_10s * 10);

		// Display hour correctly
		if (hour12 > 9){
	                buffer[1] = display_number(1);
                        buffer[3] = display_number(hour12-10);
		}else{
	      	        buffer[1] = display_number(-1);
			buffer[3] = display_number(hour12);
		}

		// Blink center colon every odd second
		if ((sec % 2) == 0){
			buffer[5] = 0b11111111;
		}else{
			buffer[5] = 0b00000000;
		}
		buffer[7] = display_number(min_10s);
		buffer[9] = display_number(min_1s);

        	result = write(fd, buffer, 17);
		sleep(1);
	}
}
