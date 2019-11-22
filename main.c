#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <linux/i2c-dev.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
	char *filename = (char*)"/dev/i2c-1";
	int fd;
	if ((fd = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return 0;
	}

	int addr = 0x70;          //<<<<<The I2C address of the slave
	if (ioctl(fd, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return 0;
	}
    uint8_t buffer[] = {0x01};

	size_t length = 1;			//<<< Number of bytes to write
	if (write(fd, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
        return 0;
	}

    return 0;
}





	// //----- READ BYTES -----
	// length = 4;			//<<< Number of bytes to read
	// if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	// {
	// 	//ERROR HANDLING: i2c transaction failed
	// 	printf("Failed to read from the i2c bus.\n");
	// }
	// else
	// {
	// 	printf("Data read: %s\n", buffer);
	// }

