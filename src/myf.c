/**
 *      @file  myf.c
 *      @brief Ficheiro que contem funções auxiliares ao funcionamento do programa.
 *      Este ficheiro contem as definições das funções que realizam as tarefas essenciais ao programa.
        Funções estas como o estabelecimento da comunicação, a escrita e leitura de dados a
        partir da porta série e escrita na shared memory.
 *
 *     Mais informações sobre o projeto em: https://github.com/TiagoSMarques/PARI-inclinometro
 *     @author  Tiago Marques,  marques.tiago19@ua.pt
 *
 *   @internal
 *     Created  28-Jan-2018
 *   Revisions  06-Feb-2018
 *
 * =====================================================================================
 */


//Funções auxiliares

#include "myf.h"
#include "prototypes.h"

/*! \brief Função que estabelece comunicação com a porta serie que corresponde à ligação bluetooth.
 *
 *
 * @param char *port_name - String com o nome físico da porta onde que está emparelhada com o dispositivo de bluetooth.
 * @param char *text - Texto a enviar quando estalelece a ligação.
 * \return Inteiro com o identificador da porta.
 */

int OpenPort(char* port_name, char* text) {

    int fd;
    fd=open(port_name,O_RDWR | O_NDELAY);

    write (fd, text, strlen(text) );

    if (text){                  /*Se existir texto para enviar entao escreve a menssagem */
      write (fd, text, strlen (text));
    }
    return fd;

}

/*! \brief Função que envia dados pela porta série para o dispositivo bluetooth emparelhado.
 *
 * Para além de ler os caracteres que chegam da porta serie a função organiza-os através da identificadores
de separação entre valores e entre linhas de dados. Os dados são depois "arrumados" numa estrutura e
posteriormente convertidos para decimais.
 * @param int fd - Inteiro com o identificador da porta
 * \return Inteiro genérico.
 */
int ReadPortUntilChar(int fd){

    char ch;
    int n, ret, count,x,exit;
    char *valSens;
    struct SensInfo Dists;

    exit=0;
    count=0;
    int end=0;
    printf("Initializing reading...\n");
    do {
        do {
            valSens = (char*) calloc(1, sizeof(char));
            x=0;
            do {
                n=read(fd, &ch, 1);
                if (n == -1) continue;
                if (n > 0) {
                    if(ch=='#'){exit=1;break;}
                    else if(ch=='$'){break;}
                    else if(ch=='!'){exit=1;end=1;break;}
                    x++;
                    valSens=realloc(valSens, sizeof(char)*x);
                    strncat(valSens, &ch,1);
                    fflush(stdout);
                }
            } while ((ch != '$') == (ch != '#'));

            count++;

            switch (count) {
                case 1:
                   //free(Dists.dist1);
                   Dists.dist1=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.dist1,valSens);
                case 2:
                   // free(Dists.dist2);
                   Dists.dist2=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.dist2,valSens);
                case 3:
                    //free(Dists.dist3);
                   Dists.dist3=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.dist3,valSens);
                case 4:
                   // free(Dists.dist4);
                   Dists.dist4=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.dist4,valSens);
                case 5:
                   // free(Dists.roll);
                   Dists.roll=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.roll,valSens);
                case 6:
                   //free(Dists.pitch);
                   Dists.pitch=(char*) calloc(1,sizeof(valSens)+1);
                   strcpy(Dists.pitch,valSens);
            }
            ch='|';
            free(valSens);
        } while (exit != 1);
        TransMain(Dists,fd);
        count=0;
        exit=0;
    } while (end!=1);

    return 0;
}

/*! \brief Função que envia uma mensagem por porta série.
 *
 *
 * @param int fd - Inteiro que identifica a porta série.
 * @param char *text - Texto a enviar quando estalelece a ligação.
 * \return Inteiro genérico.
 */
int WriteToBT(int fd, char* text){

    write(fd, text, strlen(text) );

    return 0;
}

/*! \brief Função que altera o baudrate da cominucação.
 *
 *
 * @param int fd - Inteiro que identifica a porta série.
 * @param int new_bd - Novo baudrate.
 * \return Inteiro Com o valor do antigo baudrate.
 */
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

/*! \brief Função que gera a chave para aceder à shared memory e se conecta a ela.
 *
 *
 * @param void
 * \return Inteiro com o valor do identificador da shared memory criada.
 */
int GetSharedMem(void){

    key_t key;
    int shm_id;
    key =ftok("main.c",'X');    //gera uma key random baseado num ficheiro e caracter

    //agora conectar ou escrever a shared memory com permissao 644
    shm_id = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
    if(shm_id == -1) {perror("shmget");return -1;}

    return shm_id;
}

/*! \brief Função que converte os dados recebidos para números decimais e os escreve na shared memory.
 *
 *
 * @param Struct SensInfo Dists - Estrutura de dados com as strings correspondentes às leituras dos sensores.
 * @param int fd - Inteiro que identifica a porta série.
 * \return Inteiro genérico.
 */
int TransMain(struct SensInfo Dists,int fd){

    int n, shm_id;
    struct SensInfoNum *ToShm;

    shm_id=GetSharedMem();
    if(shm_id ==-1) return-1; //failiure

    ToShm= (struct SensInfoNum *)shmat(shm_id, (void *) 0,0);
    if (ToShm == (struct SensInfoNum *) (-1)) {perror("shmat");exit(1);}

    // data agora aponta para a área partilhada
    ToShm->dist1=atof(Dists.dist1);
    ToShm->dist2=atof(Dists.dist2);
    ToShm->dist3=atof(Dists.dist3);
    ToShm->dist4=atof(Dists.dist4);
    ToShm->roll=atof(Dists.roll);
    ToShm->pitch=atof(Dists.pitch);

    ToShm->i=ToShm->i+1;
    ToShm->fd=fd;

    //detatch do segmento de memoria uma vez que estamos a sair
    if (shmdt(ToShm)==1){perror("shmt");exit(1);}

    return 0;
}

