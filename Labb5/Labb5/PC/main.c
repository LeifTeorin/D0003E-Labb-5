/*
 * PC_Code.c
 * Created: 2021-03-10 08:42:25
 * Author : Crill & Hjall
 */ 

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

//Filedirectory
int fd; 
//Lights (duh) 1 = green, 0 = red.
int LightNorth; 
int LightSouth;
//Amount in Queue
int NorthQ = 0;
int SouthQ = 0;
//Amount on bridge
int Bridgecnt = 0;
	
//Threads yeet
pthread_t rP;
pthread_t ui;
pthread_t simp;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *GUI(void *arg)
{
	pthread_mutex_lock(&m);
	printf("\033c\n"); //Cleara screen i cygwin (plz work)
	if(LightNorth)
	{
		printf("North: GREEN\n");
	}
	if(!LightNorth)
	{
		printf("North: RED\n");
	}
	printf("NorthQ: %d", NorthQ);
	printf("\n");
	if(LightSouth)
	{
		printf("South: GREEN\n");
	}
	if(!LightSouth)
	{
		printf("South: RED\n");
	}
	printf("SouthQ: %d", SouthQ);
	printf("\n");
	printf("Bridge: %d", Bridgecnt);
	printf("\n");
	if(LightNorth && LightSouth){
		printf("ohell, nu är båda gröna");
	}
	pthread_mutex_unlock(&m);
	pthread_exit(0);
}

int openPort()
{
	int speed = 9600;
	//Simulera termios
	int fd = open("/dev/ttyS0", O_RDWR);
	if (fd < 0) 
	{
		printf("oheeeeeeeeell \n");
	}
	struct termios tty;
	if (tcgetattr(fd, &tty)) 
	{
		printf("Couldn't get termios attributes! \n");
		return -1;
	}
	//Output speed
	cfsetospeed(&tty, (speed_t)speed);
	//Input speed
	cfsetispeed(&tty, (speed_t)speed);
	
	tty.c_cflag |= CLOCAL | CREAD;
	tty.c_cflag &= ~CSIZE;
	//Set 8bit
	tty.c_cflag |= CS8;
	//St�ng av parity bit
	tty.c_cflag &= ~PARENB;
	//Kr�v enbart en stop bit
	tty.c_cflag &= ~CSTOPB;
	//Ingen hardware flowcontrol
	tty.c_cflag &= ~CRTSCTS;  
	//St�ng av CANONICAL INPUT
	tty.c_lflag &= ~ICANON; 
	//St�ng av Interrupts
	tty.c_lflag &= ~ISIG; 
	//St�ng av ECHO for shits and giggles
	tty.c_lflag &= ~(ECHO | ECHOE | ECHONL | IEXTEN);
	//Ignorera inte carriage return
	tty.c_iflag &= ~IGNCR; 
	//Disabla input parity
	tty.c_iflag &= ~INPCK;
	//Disable Translations och posix stuff
	tty.c_iflag &= ~(INLCR | ICRNL | IUCLC | IMAXBEL);
	//Disabla XON/XOFF flow control, restarta ej stoppad output
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);  
	//Disable output processing
	tty.c_oflag &= ~OPOST;
	//S�tt ej end of line characters
	tty.c_cc[VEOL] = 0;
	//S�tt ej end of line characters 2.0 electric boogaloo
	tty.c_cc[VEOL2] = 0;
	//S�tt ej end of line characters the third.
	tty.c_cc[VEOF] = 0x04;
	
	//TCSANOW = immediate change
	if(tcsetattr(fd, TCSANOW, &tty) != 0)
	{
		//Printa attribute error
		printf(strerror(errno));
		return -1;
	}
	return fd;
	
}

void *readPort(void *arg)
{
	uint8_t c;
	//Get lampstatus from read bytes
	while(1)
	{
		int readBytes = read(fd, &c, sizeof(c));
		if(readBytes > 0)
		{
			LightNorth = (c & 1);
			LightSouth = (c >> 2)&1;
			pthread_create(&ui, NULL, GUI, NULL);
		}
	}
}

void writePort(uint8_t data)
{
	uint8_t x = data;
	pthread_create(&ui, NULL, GUI, NULL);
	int bytes_written = write(fd, &x, 1);
	if(fd < 0)
	{
		//Om det inte gick att printa datan
		printf("OHELL");
	}
}

int Input(void *arg)
{
	char c;
	while(1)
	{
		c = getchar();

		if(c == 'n')
		{
			NorthQ++;
			writePort(1);
		}

		if(c == 's')
		{
			SouthQ++;
			writePort(4);
		}

		if(c == 'e')
		{
			exit(0);
		}
	}
}

void* updateBridge(void *arg)
{
	Bridgecnt++;
	pthread_create(&ui, NULL, GUI, NULL);
	sleep(5);
	Bridgecnt--;
	pthread_exit(0);
}

void *Simulator(void *arg)
{
	while(1)
	{
		if( (SouthQ > 0) && LightSouth == 1)
		{
			SouthQ--;
			pthread_t drive;
			pthread_create(&drive, NULL, updateBridge, NULL);
			writePort(8);
			sleep(1);
		}
		if( (NorthQ > 0) && LightNorth == 1)
		{
			NorthQ--;
			pthread_t drive;
			pthread_create(&drive, NULL, updateBridge, NULL);
			writePort(2);
			sleep(1);
		}
	}
}

int main(void)
{
	fd = openPort();

	if(pthread_create(&rP, NULL, readPort, NULL)){printf("FAIL ON: readPort");}
	if(pthread_create(&ui, NULL, GUI, NULL)){printf("FAIL ON: Input");}
	if(pthread_create(&simp, NULL, Simulator, NULL)){printf("FAIL ON: Simulator");}

	return Input(NULL);
}