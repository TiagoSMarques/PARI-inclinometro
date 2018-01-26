//Funções auxiliares
//
#include "myf.h"

int OpenPort(char* port_name, char* text) {

    int fd;
    fd=open(port_name,O_RDWR | O_NDELAY);

    write (fd, text, strlen(text) );

    if (text){                  /*Se existir texto para enviar entao escreve a menssagem */
      write (fd, text, strlen (text));
    }
    return fd;

}

int ReadPortUntilChar(int fd){

    char ch;
    int n, ret;

    do {
        n=read(fd, &ch, 1);
        if (n == -1) continue;
        if (n > 0) { printf ("%c", ch); fflush(stdout); }
    } while (ch != '!');

    return 0;
}

int WriteToBT(int fd, char* text){

    write(fd, text, strlen(text) );

    if (strcmp(text,"exit\n") == 0) {
        return 15;
    }
    else {
        return 0;
    }
}

int ChangeBaudRate(int fd, int new_bd){

    int oldS;
    struct termios options;

    //get current options for the port
    tcgetattr(fd, &options);
    oldS=cfgetispeed(&options);

    //set the baudrate to new bdrate
    cfsetispeed(&options, new_bd);
    cfsetispeed(&options, new_bd);

    //Enable the receiver and set local mode
    options.c_cflag |= (CLOCAL | CREAD);

    //set the new options for the port
    tcsetattr(fd, TCSANOW, &options);

    return oldS;
}
