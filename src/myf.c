//Funções auxiliares
//
#include "myf.h"
#include "prototypes.h"

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
                    //printf ("%c", ch);
                    if(ch=='#'){exit=1;break;}
                    else if(ch=='$'){break;}
                    else if(ch=='!'){exit=1;end=1;break;}
                    x++;
                    valSens=realloc(valSens, sizeof(char)*x);
                    strncat(valSens, &ch,1);
                    fflush(stdout);
                }
            } while ((ch != '$') == (ch != '#'));

            //if(exit==1){break;}
            //printf("%s ",valSens);
            count++;
            //printf("count: %d \n",count);
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
            //print(" -%d", sizeof(valSens));
            free(valSens);
        } while (exit != 1);
        //free(valSens);
        TransMain(Dists);
        //printf("%s %s %s %s %s %s \n", Dists.dist1, Dists.dist2, Dists.dist3, Dists.dist4, Dists.roll, Dists.pitch);
        count=0;
        exit=0;
        //printf("\n");
    } while (end!=1);

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


}

int TransMain(struct SensInfo Dists){

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

    //ToShm->num=atoi(Dists.dist1);
    ToShm->i=ToShm->i+1;

    //printf("Transf: %f %f %f %f %f %f \n", ToShm->dist1, ToShm->dist2, ToShm->dist3, ToShm->dist4, ToShm->roll, ToShm->pitch);

    //detatch do segmento de memoria uma vez que estamos a sair
    if (shmdt(ToShm)==1){perror("shmt");exit(1);}

    return 0;
}

void destroy_Wind(GtkWidget * window, GdkEvent * event, gpointer data){
        puts("Pedido de destruição de janela");
        gtk_main_quit();  //necessary to to leave GTK main loop
}

void InterceptCTRL_C(int a) {
	   g_print("Sair por CTRL-C\n");
	   gtk_main_quit();
}

int beguin_read(GtkWidget * window, GdkEvent * event, gpointer data){

    ContRead=0;
    GtkTextView *ts = GTK_TEXT_VIEW(data);
    GtkTextBuffer *buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builderG,"sens_val")));
    if(ts) {

        int n, shm_id;
        struct SensInfoNum *ToShm;
        char write[150];

    while (ContRead==0){
        shm_id=GetSharedMem();
        if(shm_id ==-1) return-1; //failiure

        ToShm= (struct SensInfoNum *)shmat(shm_id, (void *) 0,0);
        if (ToShm == (struct SensInfoNum *) (-1)) {perror("shmat");exit(1);}

        while (gtk_events_pending ())
        gtk_main_iteration ();

       sprintf(write," %.03f %.03f %.03f %.03f %.03f %d \n", ToShm->dist1, ToShm->dist2, ToShm->dist3, ToShm->dist4, ToShm->roll, ToShm->pitch, ToShm->i);
        //printf("DDDD: %s",write);
        gtk_text_buffer_set_text (buffer, write, -1);
        sleep(1);
        memset(&write[0], 0, sizeof(write));
    }
        //detatch do segmento de memoria uma vez que estamos a sair
        if (shmdt(ToShm)==1){perror("shmt");exit(1);}
    }
}

int stop_read(GtkWidget * window, GdkEvent * event, gpointer data){

    ContRead=1;
}
