#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "extra.c"
#include "find.c"
#include "findreplace.c"
GtkWidget *view;
GdkAtom sel_atom = GDK_SELECTION_CLIPBOARD;
GtkItemFactory *main_menu;
void show_help(void);
void show_about(void); 
#define MENU_NEW 1
#define MENU_OPEN 2
#define MENU_SAVE 3
#define MENU_SAVE_AS 4
#define MENU_QUIT 5
#define MENU_CUT 6
#define MENU_COPY 7
#define MENU_PASTE 8
#define MENU_USE_CLIPBOARD 9
#define MENU_SELECT_ALL 10
#define MENU_FIND 11
#define MENU_REPLACE 12
#define MENU_HELP 14
#define MENU_ABOUT 15
static void menu_show(gpointer data, guint action, GtkWidget *widget){
	GtkTextIter p;
	switch(action){
		case MENU_NEW:
			if(save_if_modified()){
				buf = gtk_text_buffer_new(gtk_text_buffer_get_tag_table(buf));
				gtk_text_view_set_buffer(GTK_TEXT_VIEW(view), buf);
				g_object_unref(G_OBJECT(buf)); 
			}
			break;
		case MENU_OPEN:
			if(save_if_modified()) {
				buf = gtk_text_buffer_new(gtk_text_buffer_get_tag_table(buf));
				gtk_text_view_set_buffer(GTK_TEXT_VIEW(view), buf);
				g_object_unref(G_OBJECT(buf));
				load_file(NULL);
			}
			break;
		case MENU_SAVE:
			save_file(filename);
			break;
		case MENU_SAVE_AS:
			save_file(NULL);
			break;
		case MENU_QUIT:
			if(save_if_modified())
				gtk_widget_destroy(window);
			break;
		case MENU_CUT:
			gtk_text_buffer_cut_clipboard(buf,gtk_clipboard_get(sel_atom), TRUE);
			break;
		case MENU_COPY:
			gtk_text_buffer_copy_clipboard(buf,gtk_clipboard_get(sel_atom));
			break;
		case MENU_PASTE:
			gtk_text_buffer_paste_clipboard(buf,gtk_clipboard_get(sel_atom), NULL, TRUE);
			break;
		case MENU_FIND:
			textfind();
			break;
		case MENU_REPLACE:
			text_find_replace();	
			break;
		case MENU_SELECT_ALL:
			gtk_text_buffer_get_start_iter(buf, &p);
			gtk_text_buffer_place_cursor(buf, &p);
			gtk_text_buffer_get_end_iter(buf, &p);
			gtk_text_buffer_move_mark_by_name(buf, "selection_bound", &p);
			break;
      		case MENU_HELP:
			show_help();
			break;
		case MENU_ABOUT:
			show_about();
			break;
		default:
			g_printerr("Menu action not defined : %u\n", action);
			break;
	}
} 
GtkItemFactoryEntry menu_def[] = 
  {
    { (char *)"/_File", NULL, NULL, 0, (char *)"<Branch>", NULL },
    { (char *)"/File/_New", (char *)"<control>N", menu_show, MENU_NEW, (char *)"<StockItem>", GTK_STOCK_NEW },
    { (char *)"/File/_Open...", (char *)"<control>O", menu_show, MENU_OPEN, (char *)"<StockItem>", GTK_STOCK_OPEN },
    { (char *)"/File/_Save", (char *)"<control>S", menu_show, MENU_SAVE, (char *)"<StockItem>", GTK_STOCK_SAVE },
    { (char *)"/File/Save _As...", NULL, menu_show, MENU_SAVE_AS, (char *)"<StockItem>", GTK_STOCK_SAVE_AS },
    { (char *)"/File/sep", NULL, NULL, 0, (char *)"<Separator>", NULL },
    { (char *)"/File/_Quit", (char *)"<control>Q", menu_show, MENU_QUIT, (char *)"<StockItem>", GTK_STOCK_QUIT },
    { (char *)"/_Edit", NULL, NULL, 0, (char *)"<Branch>", NULL },
    { (char *)"/Edit/C_ut", (char *)"<control>X", menu_show, MENU_CUT, (char *)"<StockItem>", GTK_STOCK_CUT },
    { (char *)"/Edit/_Copy", (char *)"<control>C", menu_show, MENU_COPY, (char *)"<StockItem>", GTK_STOCK_COPY },
    { (char *)"/Edit/_Paste", (char *)"<control>V", menu_show, MENU_PASTE, (char *)"<StockItem>", GTK_STOCK_PASTE },
    { (char *)"/Edit/sep", NULL, NULL, 0, (char *)"<Separator>", NULL },
    { (char *)"/Edit/Select All", NULL, menu_show, MENU_SELECT_ALL, NULL, NULL },
    { (char *)"/_Search", NULL, NULL, 0, (char *)"<Branch>", NULL },
    { (char *)"/Search/_Find",  (char *)"<control>F" , menu_show, MENU_FIND,(char *)"<StockItem>", GTK_STOCK_FIND },
    { (char *)"/Search/_Replace", (char *)"<control>R" , menu_show, MENU_REPLACE,(char *)"<StockItem>", GTK_STOCK_FIND },
    { (char *)"/_Help", NULL, NULL, 0, (char *)"<Branch>", NULL },
    { (char *)"/Help/_Contents", (char *)"<control>H", menu_show, MENU_HELP, (char *)"<StockItem>", GTK_STOCK_HELP },
    { (char *)"/Help/_About",  NULL , menu_show, MENU_ABOUT, "<Item>" },    
  };
