#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#define CHAR_BUF 2000
GtkWidget *window;
GtkTextBuffer *buf;
char *filename;
static void load_file(char *fname){
	FILE *f;
	GtkTextIter p;
	char fbuf[CHAR_BUF];
	size_t l;    
	if(!fname){    /* create file selection dialog */
		GtkWidget *dialog = gtk_file_selection_new("Open File...");
		int resp = gtk_dialog_run(GTK_DIALOG(dialog));
		if(resp == GTK_RESPONSE_OK){
			fname = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
			gtk_widget_destroy(dialog);
		} 
		else{
			gtk_widget_destroy(dialog);
			return;
		}
	}
	if(!(f = fopen(fname, "r"))){
		g_printerr("%s: %s\n", fname, g_strerror(errno));
		return;
	}
	if(fname != filename) {
		gchar *wt = g_strdup_printf("Text Editor (%s)", fname);
		g_free(filename);
		filename = fname;
		gtk_window_set_title(GTK_WINDOW(window), wt);
		g_free(wt);
	}
	gtk_text_buffer_get_end_iter(buf, &p);
	while((l = fread(fbuf, 1, sizeof(fbuf), f)) > 0){
		GError *err = NULL;
		gsize br, bw;
		gchar *text;
		if(!(text = g_locale_to_utf8(fbuf, l, &br, &bw, &err))){
			g_printerr("File is not in UTF-8 format : %s\n", err->message);
			g_clear_error(&err);
			filename = NULL;
			gtk_window_set_title(GTK_WINDOW(window), "Text Editor (Insert file name)");
			fclose(f);
			return;
		}
		gtk_text_buffer_insert(buf, &p, text, bw);
		g_free(text);
	}
	gtk_text_buffer_set_modified(buf, FALSE);
	gtk_text_buffer_get_start_iter(buf, &p);
	gtk_text_buffer_place_cursor(buf, &p);
	if(ferror(f)){
		g_printerr("%s: %s\n", fname, g_strerror(errno));
		fclose(f);
		return;
	}
	if(fclose(f) == EOF)
		g_printerr("%s: %s\n", fname, g_strerror(errno));
}
static gboolean save_file(char *fname){
	FILE *f;
	int ok = TRUE;    
	if(!fname){
		GtkWidget *dialog = gtk_file_selection_new("Save File As...");
		int resp = gtk_dialog_run(GTK_DIALOG(dialog));
		if(resp == GTK_RESPONSE_OK) {
			fname = g_strdup(
			gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
			gtk_widget_destroy(dialog);
		}
		else{
			gtk_widget_destroy(dialog);
			return FALSE;
		}
	}
	if(!(f = fopen(fname, "w"))){
		g_printerr("%s: %s\n", fname, g_strerror(errno));
		ok = FALSE;
	}
	else{
		GtkTextIter start, end, p;
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buf), &start, &end);
		p = start;
		while(!gtk_text_iter_equal(&start, &end)) {
	    		gchar *buf, *fbuf;
	    		gsize br, bw;
	    		GError *err = NULL;
	    		gtk_text_iter_forward_chars(&p, CHAR_BUF);
	    		buf = gtk_text_iter_get_slice(&start, &p);
	    		fbuf = g_locale_from_utf8(buf, -1, &br, &bw, &err);
	    		g_free(buf);
			if(!fbuf){
				g_printerr("Failed UTF-8 to locale conversion: %s\n", err->message);
				g_clear_error(&err);
				ok = FALSE;
				break;
	    		}
			fwrite(fbuf, bw, 1, f);
			g_free(fbuf);
			if(ferror(f)){
			g_printerr("%s: %s\n", fname, g_strerror(errno));
			ok = FALSE;
			break;
		}
		start = p;
	}
	if(fclose(f) == EOF) {
		g_printerr("%s: %s\n", fname, g_strerror(errno));
		ok = FALSE;
	}
	}
	if(ok){
		gtk_text_buffer_set_modified(buf, FALSE);
		if(fname != filename) {
			gchar *wt = g_strdup_printf("TextView (%s)", fname);
			g_free(filename);
			filename = fname;
			gtk_window_set_title(GTK_WINDOW(window), wt);
			g_free(wt);
		}
	}
	return ok;
}
static gboolean save_if_modified(void){
	int resp;
	GtkWidget *dialog;
	if(!gtk_text_buffer_get_modified(GTK_TEXT_BUFFER(buf)))
		return TRUE;
	dialog = gtk_message_dialog_new(GTK_WINDOW(window), 0, GTK_MESSAGE_QUESTION, GTK_BUTTONS_NONE, "Buffer Modified. Do you want to save it now?");
	gtk_dialog_add_buttons(GTK_DIALOG(dialog), GTK_STOCK_YES, GTK_RESPONSE_YES, GTK_STOCK_NO, GTK_RESPONSE_NO, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
	resp = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	switch(resp) {
		case GTK_RESPONSE_YES:
				return save_file(filename);
		case GTK_RESPONSE_NO:
				return TRUE;
		case GTK_RESPONSE_CANCEL: case GTK_RESPONSE_DELETE_EVENT:
				return FALSE;
		default:
			g_printerr("Error response %d from dialog\n", resp);
			return FALSE;
	}
}
