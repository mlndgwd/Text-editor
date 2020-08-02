#ifdef GTK_ENABLE_BROKEN
#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "fileselect.c"
#include "menu.c"
GtkWidget *view;
//handles error
static void err_msg(const gchar *msg){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0, GTK_MESSAGE_ERROR,GTK_BUTTONS_OK, msg);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
	return !save_if_modified();
}
int main(int argc, char *argv[]){
	GtkWidget *box, *scroll, *widget, *window;   
	GtkAccelGroup *accel;  
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);  
	g_signal_connect(G_OBJECT(window), "delete_event",G_CALLBACK(delete_event), window);
	g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_title(GTK_WINDOW(window), "Text Editor (Insert file name)");
	gtk_window_set_default_size(GTK_WINDOW(window), 625, 625);    
	box = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), box);
	accel = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window), accel);
	main_menu = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<TextView>", accel);
	gtk_item_factory_create_items(main_menu,sizeof(menu_def) / sizeof(*menu_def),menu_def, NULL);
	widget = gtk_item_factory_get_widget(main_menu, "<TextView>");
	gtk_box_pack_start(GTK_BOX(box), widget, FALSE, FALSE, 0);    
	scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);
	view = gtk_text_view_new();
	buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
	gtk_container_add(GTK_CONTAINER(scroll), view);
	gtk_widget_show_all(window);
	g_set_printerr_handler(err_msg); 
	if(argc > 1)
		load_file(g_strdup(argv[1]));
	gtk_main();
	return 0;
}
#endif
