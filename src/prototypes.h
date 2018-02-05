/*File generated automatically in manjaro by tiago on Mon Feb  5 11:37:42 WET 2018*/
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
gboolean RefreshData (gpointer data);
int TransMain (struct SensInfo Dists, int fd);
int WriteToBT (int fd, char *text);
int begin_read (GtkWidget * window, GdkEvent * event, gpointer data);
int calib_sens (GtkWidget * window, GdkEvent * event, gpointer data);
void change_img (GtkWidget * window, GdkEvent * event, gpointer data);
void destroy_Wind (GtkWidget * window, GdkEvent * event, gpointer data);
int stop_read (GtkWidget * window, GdkEvent * event, gpointer data);
#ifdef __cplusplus
}
#endif
