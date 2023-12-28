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
