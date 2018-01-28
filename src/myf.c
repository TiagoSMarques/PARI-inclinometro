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
    int n, ret, count,x;
    char *valSens;
    struct SensInfo Dists;

    count=0;
    printf("Initializing reading...\n");
    do {
        do {
            valSens = (char*) calloc(1, sizeof(char));
            x=0;
            do {
                n=read(fd, &ch, 1);
                if (n == -1) continue;
                if (n > 0) {
                    //printf ("%c", ch);
                    if(ch=='$' || ch=='#'){break;}
                    x++;
                    valSens=realloc(valSens, sizeof(char)*x);
                    strncat(valSens, &ch,1);
                    fflush(stdout);
                }
            } while ((ch != '$') == (ch != '#'));
            //printf("%s ",valSens);
            count++;
            //printf("count: %d \n",count);
            switch (count) {
                case 1:
                   Dists.dist1=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.dist1,valSens);
               case 2:
                   Dists.dist2=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.dist2,valSens);
                case 3:
                   Dists.dist3=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.dist3,valSens);
                case 4:
                   Dists.dist4=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.dist4,valSens);
                case 5:
                   Dists.roll=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.roll,valSens);
                case 6:
                   Dists.pitch=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.pitch,valSens);
            }
            free(valSens);
        } while (ch != '#');
        printf("%s %s %s %s %s %s \n", Dists.dist1, Dists.dist2, Dists.dist3, Dists.dist4, Dists.roll, Dists.pitch);
        ch=' ';
        count=0;
        //printf("\n");
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

int GetSharedMem(void){

    key_t key;
    int shm_id;
    key =ftok("main.c",'X');    //gera uma key random baseado num ficheiro e caracter

    //agora conectar ou escrever a shared memory com permissao 644
    shm_id = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
    if(shm_id == -1) {perror("shmget");return -1;}

    return shm_id;
}

int GtkMain(void){

    int n,shm_id;
    char *data;     //ponteiro generico para servir de link para a shared memory
    char str[200];

    shm_id = GetSharedMem();
    if(shm_id == 1) return -1;

    //attach memory segment to get a pointer to it
    data = shmat(shm_id, (void *) 0,0);
    if (data == (char *) (-1)) {perror("shmat");exit(1);}

    // data agora aponta para a área partilhada

    /*
    Fazer operações com na shared memory
    */

    //detatch do segmento de memoria uma vez que estamos a sair
    if (shmdt(data)==1){perror("shmt");exit(1);}
}

int TransMain(void){

    int n, shm_id;
    char *data;
    char str[200]; //string to put a message

    shm_id=GetSharedMem();
    if(shm_id ==-1) return-1; //failiure

    data = shmat(shm_id, (void *) 0,0);
    if (data == (char *) (-1)) {perror("shmat");exit(1);}

    // data agora aponta para a área partilhada

    /*
    Fazer operações com na shared memory
    */

    //detatch do segmento de memoria uma vez que estamos a sair
    if (shmdt(data)==1){perror("shmt");exit(1);}
}
