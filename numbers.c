// This file creates a function to take in a number and output its binary eq. for the display
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int display_number(int num){
    switch (num) {
	case -1:
		return 0b00000000;
        case 0:
                return 0b00111111;
        case 1:
                return 0b00000110;
        case 2:
                return 0b01011011;
        case 3:
                return 0b01001111;
        case 4:
                return 0b01100110;
        case 5:
                return 0b01101101;
        case 6:
                return 0b01111101;
        case 7:
                return 0b00000111;
        case 8:
                return 0b01111111;
        case 9:
                return 0b01101111;
        default:
                return 0;
    }
}
