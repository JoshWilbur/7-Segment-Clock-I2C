void outside_loop(int num_runs){
	int delay = 100000;
	for (int t = num_runs; t > 0; t--){
		// Clear all bits, begin sequence in top left
        	buffer[1] = 0b00000001;
        	buffer[3] = 0x00;
        	buffer[5] = 0x00;
        	buffer[7] = 0x00;
        	buffer[9] = 0x00;
        	write(fd, buffer, 17);
		usleep(delay);

		// Top side (direction: left to right)
		for (int i = 1; i < 8; i = i + 2) {
			buffer[i] = 0;
			buffer[i+2] = 0b00000001;
			write(fd, buffer, 17);
			usleep(delay);
		}

		// Down to bottom
		buffer[9] = 0b00000010;
		write(fd, buffer, 17);
		usleep(delay);
		buffer[9] = 0b00000100;
        	write(fd, buffer, 17);
        	usleep(delay);

		// Bottom (direction: right to left)
        	for (int x = 11; x > 0; x = x - 2) {
		        buffer[x] = 0;
        		buffer[x-2] = 0b00001000;
        		write(fd, buffer, 17);
        		usleep(delay);
		}

		// Back up to starting position
		buffer[1] = 0b00010000;
        	write(fd, buffer, 17);
        	usleep(delay);
        	buffer[1] = 0b00100000;
        	write(fd, buffer, 17);
	        usleep(delay);
		buffer[1] = 0b00000000;
		write(fd, buffer, 17);
	}

}
