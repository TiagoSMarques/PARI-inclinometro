/**
 *      @file  main.c
 *      @brief Função principal do programa que estabelece comunicação com um dispositivo bluetooth,
 *      faz o display e animação destes numa interface GTK.
 *
 *      Nesta função é estabelecido a organização do programa. Sendo que este é constituido por um fork
 *      inicial que separa a aplicação GTk e a transmissão de dados, esta por sua vez também dividida por um fork em
 *      recepção e envio de dados.
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


//Programa principal
//
#define _MAIN_C_

#include "myf.h"
#include "prototypes.h"

int main (int argc, char *argv[]) {

    int pid_trans;
    int pid_main;
    //int s_id;
    BtEnd=0;

    pid_main=fork();
    if(pid_main == -1) { printf("Could not fork(). Exiting\n"); return -1; }

    if (pid_main!=0){       //parte da transmissão de dados

        int fd,oldS,ret=0;

        char port[30]="/dev/rfcomm1";
        char text[20]="";

        fd=OpenPort(port, "");
        oldS=ChangeBaudRate(fd,B9600);
        if (fd == -1) { printf("Error. Couldn't open port\n"); exit(1); };

        pid_trans=fork();
        if(pid_trans == -1) { printf("Could not fork(). Exiting\n"); return -1; }

        if (pid_trans == 0 ) {
            //printf("this is the child \n");
            int shm_id;
            //Acesso à shared memory
            struct SensInfoNum *FromShm;   //ponteiro generico para servir de link para a shared memory
            shm_id = GetSharedMem();
            if(shm_id == 1) return -1;

            //attach memory segment to get a pointer to it
            FromShm =(struct SensInfoNum *) shmat(shm_id, (void *) 0,0);
            if (FromShm == (struct SensInfoNum *) (-1)) {perror("shmat");exit(1);}
            FromShm->BtWrite=0;

            while (BtEnd != 1 ) {
                if(FromShm->BtWrite==1){
                //fgets(text, sizeof(text), stdin);i
                sprintf(text,FromShm->BtText);
                ret=WriteToBT(fd, text);
                printf("end: %i\n",BtEnd);
                sleep(1);
                FromShm->BtWrite=0;
                }
            }
            if (shmdt(FromShm)==1){perror("shmt");exit(1);}
            printf("terminado filho 2\n");
        }
        else {
            //printf("this is the parent\n");
            sleep(2); //required to make flush work, for some reason
            tcflush(fd,TCIOFLUSH);  //limpar o buffer de comunicações antigas
            ReadPortUntilChar(fd);
            printf("Comunication ended!\n");
            close(fd);
        }
    }
    else {              //parte da aplicação gtk
        printf("Filho principal\n");
        int term=0;
        int shm_id;

        //Acesso à shared memory
        struct SensInfoNum *FromShm;   //ponteiro generico para servir de link para a shared memory
        shm_id = GetSharedMem();
        if(shm_id == 1) return -1;

        //attach memory segment to get a pointer to it
        FromShm =(struct SensInfoNum *) shmat(shm_id, (void *) 0,0);
        if (FromShm == (struct SensInfoNum *) (-1)) {perror("shmat");exit(1);}

        //reinicializar o contador e esperar 3segundos para que se de inicio a leitura
        FromShm->i=0;
        sleep(3);

        gtk_init(&argc, &argv);

        /* load the interface after a configuration file*/
        builderG = gtk_builder_new();
        gtk_builder_add_from_file(builderG,"../srcutils/GtkApp.glade", NULL);

        /* connect the signals in the interface */
        gtk_builder_connect_signals(builderG, NULL);

        /* get main window Widget ID and connect special signals */
        GtkWidget *t = GTK_WIDGET(gtk_builder_get_object(builderG, "window1"));
        if(t) {
            g_signal_connect(G_OBJECT(t), "delete_event", G_CALLBACK(destroy_Wind), NULL);
        }

        // use signal to catch SIGINT  (CTRL-C)
        signal(SIGINT, InterceptCTRL_C);
   //     /* Hand control over to the main loop. */
        gtk_main();

        FromShm->BtWrite=1;
        //FromShm->BtText="exit";
        sprintf(FromShm->BtText,"exit");

        sleep(2);

        if (shmdt(FromShm)==1){perror("shmt");exit(1);}
        printf("terminado filho 1\n");
        //Allow elimination of shared memory
        if(shm_id > 0) shmctl(shm_id, IPC_RMID, NULL);
    }

    return 0;
}
