#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
void show_about(){
	GtkWidget *window1;
	GtkWidget *label;
	window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window1), "About Text Editor");
	gtk_window_set_default_size(GTK_WINDOW(window1), 321, 321);
  	label = gtk_label_new ("Text Editor(DSA Miniproject)\nUsing Gtk & C");  
	gtk_container_add (GTK_CONTAINER (window1), label);
	gtk_widget_show_all (window1);
}
void show_help(void){
	GtkWidget *window1;
	GtkWidget *label;
	char help[5000];
	window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window1), "Help - Text Editor");
	gtk_window_set_default_size(GTK_WINDOW(window1), 300, 300);
	strcpy(help,"\nTo Create a New file, choose file -> New. The application displays a new file in the text editor.\n\nTo Save a File\n - To save changes in an existing file, choose File -> Save.\n - To save a new file or to save an existing file under a new filename, choose File -> Save As. Enter a name for the file in the Save As dialog, then click Save.\n\nTo Edit text\n\n - To copy the selected text to a buffer, choose Edit -> Copy.\n - To delete the selected text from the file and move the selected text to a buffer, choose Edit -> Cut.\n - To insert the contents of the buffer at the cursor position, choose Edit -> Paste.You must cut or copy text before you can paste text into the file.\n - To select all of the text in the file, choose Edit -> Select All.\n\nTo Find Text\n\n - Choose Search -> Find to display the Find dialog.\n-Type the string that you want to find, in the text box.\n- Click Find to search the file for the first occurence of the string.\n If the editor finds the string, application moves the cursor position to the string, and selects the string.\n- To find the next occurence of the string, click Next\n\nTo Find and Replace Text\n\nTo search a file for a string, and replace the string with an alternative string, perform the following steps:\n\n-Choose Search -> Replace to display the Replace dialog.\n-Type the string that you want to find, in the Search For field.\n-Click Find to search the file for the first occurence of the string.\n  If the editor finds the string, application moves the cursor position to the string, and selects the string.\n- Click Replace to replace the occurences of the string with the text in the Replace with field. \n- To find the next occurence of the string,click Next.");
	strcat(help,"\0");
	label = gtk_label_new (help);
	gtk_container_add (GTK_CONTAINER (window1), label);
	gtk_widget_show_all (window1);
}
