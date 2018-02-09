/**
 *      @file  callbacks.c
 *      @brief Ficheiro que contem as callbacks a serem usadas pela aplicação GTK
 *
 *          Este ficheiro contem as funções que estão associadas à aplicação GTK.
 *          Ou seja que irão ser chamadas por ações efetuadas pelo utilizador na aplicação.
 *
 *      Mais informações sobre o projeto em: https://github.com/TiagoSMarques/PARI-inclinometro
 *     @author  Tiago Marques,  marques.tiago19@ua.pt
 *
 *   @internal
 *     Created  28-Jan-2018
 *   Revisions  06-Feb-2018
 *
 * =====================================================================================
 */

#include "myf.h"
#include "prototypes.h"

/** @brief Função que fecha a janela GTK e termina os processos ativos
 *
 * @param GtkWidget *Window - Widget GTK associado à função
 * @param GdkEvent *event
 * @param gpointer data - Ponteiro gtk para os objetos associados à aplicação
 * @return Return void
 */

void destroy_Wind(GtkWidget * window, GdkEvent * event, gpointer data){
        ContRead=1;
        BtEnd=1;
        puts("Pedido de destruição de janela");
        gtk_main_quit();  //necessary to to leave GTK main loop
}

/*! \brief Interceptar o comando sinal de saida Ctrl+C e sair da aplicação GTK
 *
 * \param Inteiro - parâmetro requerido pela função que chama esta rotina.
 * \return void
 */
void InterceptCTRL_C(int a) {
	   g_print("Sair por CTRL-C\n");
	   gtk_main_quit();
}

/**
 * @brief  Rotina que chama periodicamente a função  que atualiza os dados mostrados na aplicação GTK
 *
 * Chama a função de atualização de segundo a segundo.
 *
 * @param GtkWidget *window - Widget GTK associado à função
 * @param GdkEvent *event
 * @param gpointer data - Ponteiro gtk para os objetos associados à aplicação.
 * @return Inteiro que representa que a função foi executada.
 */

int begin_read(GtkWidget * window, GdkEvent * event, gpointer data){

    ContRead=g_timeout_add(1000,RefreshData,data);
}

/*! \brief Função que atualiza os dados mostrados na aplicação
 *
 *  Esta função tem 3 principais funções. Acessa os dados dos sensores presentes na shared memory.
 *  Faz os display destes na aplicação GTK. E a partir desses dados seleciona a imagem adquada, representativa da inclinação do veículo.
 * Adicionalmente também escreve as leituras num ficheiro .txt
 *
 * \param gpointer data - Parâmetro que contem a informação sobre os objetos da aplicação GTK.
 * \return gboolean - Retorna TRUE para sinalizar a continuação da leitura.
 */
gboolean RefreshData(gpointer data){

    GtkTextView *ts = GTK_TEXT_VIEW(data);
    GtkTextBuffer *buffer_dist=gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builderG,"sens_val")));
    GtkTextBuffer *buffer_roll=gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builderG,"text_roll")));
    GtkTextBuffer *buffer_pitch=gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builderG,"text_pitch")));

    //ESCRITA NA SHARED MEMORY
    int n, shm_id;
    struct SensInfoNum *ToShm;
    char write[100];
    char roll[50];
    char pitch[50];
    FILE *f=fopen("sensores_data.txt","a+");
    if (f == NULL){printf("Error opening file!\n");exit(1);}

    shm_id=GetSharedMem();
    if(shm_id ==-1) return-1; //failiure

    ToShm= (struct SensInfoNum *)shmat(shm_id, (void *) 0,0);
    if (ToShm == (struct SensInfoNum *) (-1)) {perror("shmat");exit(1);}

    sprintf(write," %.03f | %.03f | %.03f | %.03f -%d", ToShm->dist1, ToShm->dist2, ToShm->dist3, ToShm->dist4, ToShm->i);
    sprintf(roll," %.03f ", ToShm->roll);
    sprintf(pitch," %.03f ",ToShm->pitch);

    //write to file
    fprintf(f,"%s Roll: %s, Pitch: %s\n",write,roll,pitch);

    GtkImage *image_pitch=GTK_IMAGE(gtk_builder_get_object(builderG, "pitch_img"));
    GtkImage *image_roll=GTK_IMAGE(gtk_builder_get_object(builderG, "roll_img"));
    //gchar* filename;
    gchar filename1[50];
    gchar filename2[50];
    int order;
    float aux,spaces;

    //Escolha de imagens para pitch
    if (ToShm->pitch>=0){
        spaces=2.0/5.0;
        aux=(ToShm->pitch)/spaces;
        order=ceil(aux);
        if(order>5){order=5;}
        if(ToShm->pitch<=0.05){order=0;}
        g_snprintf(filename1,50,"../srcutils/img/vista_lado%i-.png",order);
    }
    else{
        spaces=2.0/7.0;
        aux=ToShm->pitch/spaces;
        order=ceil(abs(aux));
        if(ToShm->pitch>=-0.05){order=0;}
        if (order>7){order=7;}
        g_snprintf(filename1,50,"../srcutils/img/vista_lado%i+.png",order);
     }

    //Escolha de imagem para o Roll

    if (ToShm->roll>=0){
        spaces=2.0/5.0;
        aux=(ToShm->roll)/spaces;
        order=ceil(aux);
        if(ToShm->roll<=0.05){order=0;}
        if(order>5){order=5;}
        g_snprintf(filename2,50,"../srcutils/img/vista_tras%i-.png",order);
    }
    else{
        spaces=2.0/4.0;
        aux=ToShm->pitch/spaces;
        order=ceil(abs(aux));
        if(ToShm->roll>=-0.05){order=0;}
        if (order>4){order=4;}
        g_snprintf(filename2,50,"../srcutils/img/vista_tras%i+.png",order);
    }

    //Set pitch and roll img in gtk
    gtk_image_set_from_file (image_pitch,filename1);
    gtk_image_set_from_file (image_roll,filename2);
    //close file
    fclose(f);

    gtk_text_buffer_set_text (buffer_dist, write, -1);
    gtk_text_buffer_set_text (buffer_roll, roll, -1);
    gtk_text_buffer_set_text (buffer_pitch, pitch, -1);

    memset(&write[0], 0, sizeof(write));
    memset(&roll[0], 0, sizeof(roll));
    memset(&pitch[0], 0, sizeof(pitch));

    //detatch do segmento de memoria uma vez que estamos a sair
    if (shmdt(ToShm)==1){perror("shmt");exit(1);}

    return TRUE;
}


/*! \brief Função que interrompe o display dos dados na aplicação GTK.
 *
 *
 * @param GtkWidget *window - Widget GTK associado à função
 * @param GdkEvent *event
 * @param gpointer data - Ponteiro gtk para os objetos associados à aplicação.
 * \return Inteiro que confirma a execução da função.
 */
int stop_read(GtkWidget * window, GdkEvent * event, gpointer data){

    g_source_remove(ContRead);
}

/*! \brief Função que dá a instrução de envio do parâmetro de calibração.
 *
 *  Escreve na estrutura de dados da shered memory a intenção de enviar uma mensagem para o arduino e
 *  escreve também o conteúdo dessa mensagem.
 *
 * @param GtkWidget *window - Widget GTK associado à função
 * @param GdkEvent *event
 * @param gpointer data - Ponteiro gtk para os objetos associados à aplicação.
 * \return Inteiro que confirma a execução da função.
 */
int calib_sens(GtkWidget * window, GdkEvent * event, gpointer data){

    int shm_id;
    struct SensInfoNum *FromShm;

    shm_id=GetSharedMem();
    if(shm_id ==-1) return-1; //failiure

    FromShm= (struct SensInfoNum *)shmat(shm_id, (void *) 0,0);
    if (FromShm == (struct SensInfoNum *) (-1)) {perror("shmat");exit(1);}

    FromShm->BtWrite=1;
    sprintf(FromShm->BtText,"C");
    if (shmdt(FromShm)==1){perror("shmt");exit(1);}
    }
