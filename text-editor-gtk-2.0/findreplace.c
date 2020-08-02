#include <gtk/gtk.h>
#include<string.h>
extern GtkWidget *view;
GtkWidget *text_view;
GtkWidget *search_entry,*replace_entry;
void replace (GtkTextView *text_view, const gchar *text,const gchar *text1, GtkTextIter *iter){
	GtkTextIter mstart, mend;
	GtkTextBuffer *buffer;
	gboolean found;  
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(text_view));
	found = gtk_text_iter_forward_search (iter, text, 0, &mstart, &mend, NULL);
	if (found){
		gtk_text_buffer_select_range (buffer, &mstart, &mend);
		gtk_text_buffer_create_mark (buffer, "last_pos", &mend, FALSE);
		int len=strlen(text1);
		gtk_text_buffer_delete(buffer,&mstart,&mend);
		gtk_text_buffer_insert(buffer,&mstart,text1,len);
	}
}
void replace_button_clicked(GtkWidget *replace_button){
	GtkTextBuffer *buffer;
	GtkTextIter iter; 
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	gtk_text_buffer_get_start_iter(buffer, &iter);  
  replace (GTK_TEXT_VIEW(text_view),(gchar *)gtk_entry_get_text((GtkEntry *)search_entry),(gchar *)gtk_entry_get_text((GtkEntry *)replace_entry),&iter);
}
void text_find_replace(void){
	GtkWidget *win;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *search_button;
	GtkWidget *replace_button;
	win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	vbox = gtk_vbox_new (FALSE, 2);
	gtk_container_add (GTK_CONTAINER (win), vbox);
	hbox = gtk_hbox_new (FALSE, 2);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);  
	text_view = view;
	search_entry = gtk_entry_new();
	replace_entry = gtk_entry_new();
	gtk_box_pack_start (GTK_BOX (hbox),search_entry, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox),replace_entry, TRUE, TRUE, 0);
	search_button = gtk_button_new_with_label ("Search");
	gtk_box_pack_start (GTK_BOX (hbox), search_button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT (search_button), "clicked",G_CALLBACK (search_button_clicked), NULL);  
	replace_button = gtk_button_new_with_label ("Replace");
	gtk_box_pack_start (GTK_BOX (hbox), replace_button, FALSE, FALSE, 0);
	g_signal_connect (G_OBJECT (replace_button),"clicked",G_CALLBACK (replace_button_clicked), NULL);
	gtk_widget_show_all(win);
}
