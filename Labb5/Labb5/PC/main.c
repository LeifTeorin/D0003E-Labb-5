/*
 * PC_Code.c
 * Created: 2021-03-10 08:42:25
 * Author : Crill
 */ 


/*
Likewise, we will use the transmitter side of the USART for implementing the four output signals that control the status of each lamp of the traffic lights. The four signals will map onto the bits of the transmit data register as follows:


*/

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
//POSIX
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define SERIAL_PORT "/dev/ttyS0"

enum Z {
	//NONE = 0,
	RED = 0,
	GREEN = 1
};

int yeet; // yeet blir vår filedescriptor tror jag
//Trafikljusen
int LightNorth; //0 = Röd; 1 = Grön.
int LightSouth; //0 = Röd; 1 = Grön.
int writeBit[4] = {0, 0, 0, 0};
int NorthQ = 0;
int SouthQ = 0;
int Bridgecnt = 0;
int speed = 96000;
enum Z light = RED;
pthread_t readinputs;
pthread_t sim;
pthread_t readinputs;
/*
Bit 0: Northbound green light status
Bit 1: Northbound red light status
Bit 2: Southbound green light status
Bit 3: Southbound red light status
*/
void *GUI(void *arg)
{
		//Cleara sk�rmen?
//		
	while(1)
	{
		printf("\e[1;1H\e[2J");
		if(LightNorth)
		{
			printf("North: GREEN");
		}
		if(!LightNorth)
		{
			printf("North: RED");
		}
		printf("\n");
		printf("NorthQ: %d", NorthQ);
		printf("\n");
		if(LightSouth)
		{
			printf("South: GREEN");
		}
		if(!LightSouth)
		{
			printf("South: RED");
		}
		printf("\n");
		printf("SouthQ: %d", SouthQ);
		printf("\n");
		usleep(1000000);
		if(LightNorth && LightSouth){
			printf("ohell, nu är båda gröna");
		}
	}
}

int openPort()
{
	//Simulera termios

	/*int fd = open("dev/ttyS0", O_RDWR|O_NOCTTY|O_SYNC);
	struct termios tty;
	if(tcgetattr(fd, &tty) < 0)
	{
		//Skriver ut felkoden fr�n tcgetattr
		printf(strerror(errno));
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
	
	///////////////////////
	//IDK OM DESSA BEH�VS//
	///////////////////////
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
	*/
	int fd = open("/dev/ttyS0", O_RDWR);
	if (fd < 0) {
		printf("Can't open file! \n");
	}


	struct termios termios_p;
	if (tcgetattr(fd, &termios_p)) {
		printf("Couldn't get termios attributes! \n");
		return -1;
	}

	//Flush COM1 recieved data that is not read
	tcflush(fd, TCIFLUSH); 
	//Baud rate(signal changes per second) = 9600 | 
	//Character size mask 8 | 
	//Set two stop bits rather than one | 
	//Enable receiver | 
	//Ignore modem control lines | 
	//Lower modem control lines after last process closes the device (hang up) | 
	//Enable input parity checking
	termios_p.c_cflag = B9600 | CS8 | CSTOPB | CREAD | CLOCAL | HUPCL | INPCK;
	// NOT (Echo input characters |
	//echo the NL character even if ECHO is not set)
	termios_p.c_lflag &= ~(ECHO | ECHONL | ICANON);
	// Avoid inter-message overlap.
	termios_p.c_cc[VTIME] = 10;
	termios_p.c_cc[VMIN] = 1;
	cfsetispeed(&termios_p, B9600);
	cfsetospeed(&termios_p, B9600);


	if (tcsetattr(fd, TCSANOW, &termios_p)) {
		printf("Could not set termios attributes! \n");
		return -1;
	}
	return fd;
}

void *readPort(void *arg)
{
	//Read from serial port
	//F� LAMPSTATUS
	uint8_t c;
	while(1)
	{
		int readBytes = read(yeet, &c, sizeof(c));
		if(readBytes > 0)
		{
			if((readBytes&3)==2){
				LightNorth = 0;
			}
			if((readBytes&3)==1){
				LightNorth = 1;
			}
			if((readBytes&12)==4){
				LightSouth = 1;
			}
			if((readBytes&12)==8){
				LightSouth = 0;
			}
//			GUI(NULL);
		}
	}
}

void writePort(uint8_t data)
{
	int bytes_written = write(yeet, &data, 1);
	if(yeet < 0)
	{
		//Om det inte gick att printa datan
		printf("OHELL");
	}
}

void *Input(void *arg)
{
	char c;
	//Keyboard inputs
	// här ska vi köra select tror jag
	while(1){
		c = getchar();

		if(c == 'n'){
			NorthQ++;
			writePort(1);
		}

		if(c == 's'){
			SouthQ++;
			writePort(4);
		}

		if(c == 'e'){

		}
	}
}

//Uppdatera bron och se om det finns n�got p� den
void enterBridge()
{
	Bridgecnt++;
	// do some gui magic
	// skriv till avr
	sleep(5);
	Bridgecnt--;
}




/*
Om något åker in i bron
Direction 1 = north, 0 = south
Bit 1: Northbound bridge entry sensor activated
Bit 3: Southbound bridge entry sensor activated
*/
void entrySensor(int dir)
{
	if(dir == 1)
	{
		writeBit[1] = 1;
		writeBit[3] = 0;
	}
	else if (dir == 0)
	{
		writeBit[1] = 0;
		writeBit[3] = 1;
	}
}

void* updateBridge(void *arg)
{
	Bridgecnt++;
	sleep(5);
	Bridgecnt--;
	pthread_exit(0);
}

/*
void simulator(void){
	northQ = 0;
	southQ = 0;
	bridgecnt = 0;
	while(1){

		if(LightNorth && (northQ > 0)){
			northQ--;
			GUI();
			pthread_t cartobridge;
			pthread_create(&cartobridge, NULL, enterBridge, NULL);
			entrySensor(1);
//			antingen fixar denna gui eller så tar bron den
//			enterBridge();
			sleep(1);
		}

		if(LightSouth && (southQ > 0)){
			southQ--;
			GUI();
			pthread_t cartobridge;
			pthread_create(&cartobridge, NULL, enterBridge, NULL);
//			enterBridge();
			entrySensor(0):
			sleep(1);
		}

	}
}*/

//SIMULATOR IS MY PROPERTY
//TRESPASSERS WILL BE SHOT ON SIGHT
//BEGONE MEDDLERS
void *Simulator(void *arg)
{
	while(1)
	{
		//D� s�der ljus �r gr�n (1) och det finns bilar i k�n
		while( (SouthQ > 0) && LightSouth == 1 && LightNorth != 1)
		{
			SouthQ--;
			pthread_t drive;
			pthread_create(&drive, NULL, updateBridge, NULL);
			entrySensor(1);
			writePort(0x8);
			sleep(1);
		}
		while( (NorthQ > 0) && LightNorth == 1 && LightSouth != 1)
		{
			SouthQ--;
			pthread_t drive;
			pthread_create(&drive, NULL, updateBridge, NULL);
			entrySensor(0);
			writePort(0x2);
			sleep(1);
		}
		//0x8 ska vara south, 0x2 ska vara north.
		//flusha bort output buffern
		fflush(stdin);
	}
}

/*
//returnar 1 om tom
int checkBridge()
{
	if(bridgecnt > 0)
	{
		printf("Someone's on the bridge you guys\n");
		return 0;
	}
	else
	{
		printf("Tomt");
		return 1;
	}
}
*/
/*
Om n�gon kommer till bron
Bit 0: Northbound car arrival sensor activated
Bit 2: Southbound car arrival sensor activated
*/
void arrivalSensor(int dir)
{
	if(dir == 1)
	{
		writeBit[0] = 1;
		writeBit[2] = 0;
	}
	else if (dir == 0)
	{
		writeBit[0] = 0;
		writeBit[2] = 1;
	}
}

int main(void)
{
	yeet = openPort();
	pthread_t rP;
	pthread_t inp;
	pthread_t simp;


	if(pthread_create(&rP, NULL, readPort, NULL)){printf("FAIL ON: readPort");}
	if(pthread_create(&inp, NULL, Input, NULL)){printf("FAIL ON: Input");}
	if(pthread_create(&simp, NULL, Simulator, NULL)){printf("FAIL ON: Simulator");}

	
	GUI(NULL);
	//pthread_join(rP, NULL);
	//pthread_join(inp, NULL);
	//pthread_join(simp, NULL);
}