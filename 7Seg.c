#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <time.h>

#include "7Seg_setup.c"
#include "animations.c"
#include "weather_stats.c"

// Global variables
time_t current_time;
struct tm *now;

// Function prototypes
void set_clock(int num1, int num2, int num3, int num4);

int main(){
        ht16k33_setup();
	buffer[0] = 0;
	outside_loop(3);
	int sec, min, min_1s, min_10s, hour24, hour12, temp, temp_1s, temp_10s = 0;
	while(1){
		// Update time;
		time(&current_time);
		now = localtime(&current_time);

		// Obtain second, minute an hour values from time struct
		sec = now->tm_sec;
        	min = now->tm_min;
        	hour24 = now->tm_hour;
		hour12 = hour24;

		// Power saving during typical sleep hours (10pm-7am)
		if (hour24 >= 22 || hour24 <= 6){
			buffer[5] = display_number(-1);
			set_clock(-1, -1, -1, -1);
			result = write(fd, buffer, 17);
			sleep(600); // Sleep for 10 minutes
		}else{
			// Display temperature every 15 mins
			if ((min % 15 == 0) && (sec == 15)){
				temp = get_temp(); // Obtain temperature
				temp_10s = (temp / 10);
				temp_1s = temp - (temp_10s * 10);
				set_clock(-1, -1, temp_10s, temp_1s);
				result = write(fd, buffer, 17);
				sleep(30); // Display temperature for 30 seconds
			}

			// Change hour value to 12 hr
			if (hour24 > 12) hour12 = hour24 - 12;

			// Obtain 10s and 1s values of minute
			min_10s = (min / 10);
			min_1s = min - (min_10s * 10);

			// Display hour correctly
			if (hour12 > 9){
				set_clock(1, (hour12-10), min_10s, min_1s);
			}else if (hour12 == 0){
				set_clock(1, 2, min_10s, min_1s);
			}else{
				set_clock(-1, hour12, min_10s, min_1s);
			}

			// Blink center colon every odd second
			if ((sec % 2) == 0){
				buffer[5] = 0b11111111;
			}else{
				buffer[5] = 0b00000000;
			}
        		result = write(fd, buffer, 17);
			sleep(1);
		}
	}
}

// This function sets the clock to a desired value, center colon is ignored here
void set_clock(int num1, int num2, int num3, int num4){
        buffer[1] = display_number(num1);
        buffer[3] = display_number(num2);
        buffer[7] = display_number(num3);
        buffer[9] = display_number(num4);
	result = write(fd, buffer, 17);
}
