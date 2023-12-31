#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <time.h>

#include "7Seg_setup.c"
#include "numbers.c"
#include "animations.c"

// Global variables
time_t current_time;
struct tm *now;

int main(){
        ht16k33_setup();
	buffer[0] = 0;
	outside_loop(3);
	int sec, min, min_1s, min_10s, hour24, hour12 = 0;
	while(1){
		// Update time;
		time(&current_time);
		now = localtime(&current_time);

		// Obtain second, minute an hour values from time struct
		sec = now->tm_sec;
        	min = now->tm_min;
        	hour24 = now->tm_hour;
		hour12 = hour24;

		// At the beginning of every new hour, do a loop around the outside of the display
		if (min == 0 && sec == 0) outside_loop(2);

		// Change hour value to 12 hr
		if (hour24 > 12) hour12 = hour24 - 12;

		// Obtain 10s and 1s values of minute
		min_10s = (min / 10);
		min_1s = min - (min_10s * 10);

		// Display hour correctly
		if (hour12 > 9){
	                buffer[1] = display_number(1);
                        buffer[3] = display_number(hour12-10);
		}else if (hour12 == 0){
			buffer[1] = display_number(1);
                        buffer[3] = display_number(2);
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
