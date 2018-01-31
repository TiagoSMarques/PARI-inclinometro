/*File generated automatically in manjaro by tiago on Wed Jan 31 20:31:30 WET 2018*/
#ifdef __cplusplus
extern "C" {
#endif
/* main.c */
int main (int argc, char *argv[]);
/* myf.c */
int ChangeBaudRate (int fd, int new_bd);
int GetSharedMem (void);
int GtkMain (void);
void InterceptCTRL_C (int a);
int OpenPort (char *port_name, char *text);
int ReadPortUntilChar (int fd);
int TransMain (struct SensInfo Dists);
int WriteToBT (int fd, char *text);
void destroy_Wind (GtkWidget * window, GdkEvent * event, gpointer data);
void sens_val (GtkWidget * window, GdkEvent * event, gpointer data);
#ifdef __cplusplus
}
#endif
