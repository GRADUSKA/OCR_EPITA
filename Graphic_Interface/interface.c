#include <gtk/gtk.h>


typedef enum StateButton
{
    SHOWN,
    HIDDEN,
    SUDOKU,
    ROTATION,
    GRAYSCALE,
    GAUSSIAN_BLUR,
    CANNY,
    SOLVED,
    SAVE,
}StateButton;


typedef struct UserInterface
{
    GtkWindow* window;
    GtkFixed* fixed;
    GtkButton* previous_step_button;
    GtkButton* next_step_button;
    GtkButton* final_step_button;
    GtkButton* quit_button;
    GtkWidget* download_button;
    GtkButton* solve_button;
    GtkImage* sudoku_image;
    GtkImage* logo_image;
    GtkImage* background;
    GtkImage* title;
}UserInterface;


typedef struct All
{
    StateButton state1; //previous
    StateButton state2; //next
    StateButton state3; //final
    StateButton state4; //download
    gchar* sudoku_file;
    UserInterface ui;
}All;


void set_hidden4(All* all) // download button will be hidden + other button will appear
{
    all->state4 = HIDDEN;
    gtk_widget_hide(all->ui.download_button);

    gtk_widget_show(GTK_WIDGET(all->ui.previous_step_button));
    all->state1 = SUDOKU;
    gtk_widget_show(GTK_WIDGET(all->ui.next_step_button));
    all->state2 = SUDOKU;
    gtk_widget_show(GTK_WIDGET(all->ui.final_step_button));
    all->state3 = SOLVED;
}


void set_shown4(All *all) //download button will be shown + other button will disapppear
{
    all->state4 = SHOWN;
    gtk_widget_show(all->ui.download_button);

    gtk_widget_hide(GTK_WIDGET(all->ui.previous_step_button));
    all->state1 = HIDDEN;
    gtk_widget_hide(GTK_WIDGET(all->ui.next_step_button));
    all->state2 = HIDDEN;
    gtk_widget_hide(GTK_WIDGET(all->ui.final_step_button));
    all->state3 = HIDDEN;
}

void sizing_image(GdkPixbuf* pixbuf, GtkImage* image, int size)
{


    pixbuf = gdk_pixbuf_scale_simple(pixbuf, size, size, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(image, pixbuf);
}


void set_save3(All* all) // the final step button becomes the save button and next step button disappear
{
    all->state1 = SAVE;
    all->state2 = SAVE;
    all->state3 = SAVE;

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("image_test/solved.png", NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);

    gtk_button_set_label(all->ui.final_step_button, "Save");
    
    gtk_widget_hide(GTK_WIDGET(all->ui.next_step_button));
}


void set_back3(All* all)
{
    //funtion to save the the sudoku solved file
    //TODO
    set_shown4(all);
}


void set_sudoku(All* all)
{
    all->state1 = SUDOKU;
    all->state2 = SUDOKU;

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file(all->sudoku_file, NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);
}


void set_grayscale(All* all)
{
    all->state1 = GRAYSCALE;
    all->state2 = GRAYSCALE;

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("image_test/test_grayscale.bmp", NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);
}


void set_rotation(All* all)
{
    all->state1 = ROTATION;
    all->state2 = ROTATION;

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("image_test/rotation.jpg", NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);
}


void set_gaussian_blur(All* all)
{
    all->state1 = GAUSSIAN_BLUR;
    all->state2 = GAUSSIAN_BLUR;

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("image_test/test_gaussian_blur.bmp", NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);
}


void set_canny(All* all)
{
    all->state1 = CANNY;
    all->state2 = CANNY;

    gtk_widget_show(GTK_WIDGET(all->ui.next_step_button));
    gtk_button_set_label(all->ui.final_step_button, "Final step");

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("image_test/test_canny.bmp", NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);
}


void on_state_psb(GtkButton *button, gpointer user_data) //previous step button
{
    All* all = user_data;

    switch(all->state1) // previous
    {
        case SUDOKU: set_shown4(all); break;
        case ROTATION: set_sudoku(all); break;
        case GRAYSCALE: set_rotation(all); break;
        case GAUSSIAN_BLUR: set_grayscale(all); break;
        case CANNY: set_gaussian_blur(all); break;
        case SAVE: set_canny(all); break;
        default: break;
    };
}


void on_state_nsb(GtkButton *button, gpointer user_data) // next step button
{
    All* all = user_data;

    switch(all->state2) // next
    {
        case SUDOKU: set_rotation(all); break;
        case ROTATION: set_grayscale(all); break;
        case GRAYSCALE: set_gaussian_blur(all); break;
        case GAUSSIAN_BLUR: set_canny(all); break;
        case CANNY: set_save3(all); break;
        default: break;
    };
}


void on_state_fsb(GtkButton *button, gpointer user_data) // final step button
{
    All* all = user_data;

    switch(all->state3) // final
    {
        case SOLVED: set_save3(all); break;
        case SAVE: set_back3(all); break;
        default: break;
    };
}


void on_state_db(GtkButton *button, gpointer user_data) // download button
{
    All* all = user_data;

    switch(all->state4) // download
    {
        case SHOWN: set_hidden4(all); break;
        case HIDDEN: set_shown4(all); break;
        default: break;
    };
}

static void load_the_css(char* name);

void begin(GtkButton *button, gpointer user_data)
{

    All* all = user_data;

    gtk_widget_show(GTK_WIDGET(all->ui.download_button));
    gtk_widget_show(GTK_WIDGET(all->ui.logo_image));
    gtk_widget_show(GTK_WIDGET(all->ui.sudoku_image));
    gtk_widget_show(GTK_WIDGET(all->ui.title));
    gtk_widget_hide(GTK_WIDGET(all->ui.solve_button));
    gtk_widget_hide(GTK_WIDGET(all->ui.background));

    load_the_css("color.css");

}

static void load_the_css(char* name)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    GFile *css_file             = g_file_new_for_path(name);
    GError *error               = 0;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider, css_file, &error);

    g_object_unref(provider);
}


void file_selected_changed(GtkFileChooser *chooser, gpointer user_data)
{
    All *all = user_data;

    all->sudoku_file = gtk_file_chooser_get_filename(chooser);
    if(!all->sudoku_file) return;

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file(all->sudoku_file, NULL);
    GdkPixbuf* pix = gdk_pixbuf_scale_simple(pixbuf, 800, 800, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(all->ui.sudoku_image, pix);

    set_hidden4(all);
}


int main (int argc, char *argv[])
{
    gtk_init(NULL, NULL);

    load_the_css("bcolor.css");

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkFixed* fixed = GTK_FIXED(gtk_builder_get_object(builder, "fixed"));

    GtkImage* title = GTK_IMAGE(gtk_builder_get_object(builder, "title"));
    GdkPixbuf* pix2 = gdk_pixbuf_new_from_file("image_test/title.png", NULL);
    pix2 = gdk_pixbuf_scale_simple(pix2, 700, 130, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(title, pix2);


    GtkImage* background = GTK_IMAGE(gtk_builder_get_object(builder, "background"));
    GdkPixbuf* pixbuf2 = gdk_pixbuf_new_from_file("image_test/background.png", NULL);
    pixbuf2 = gdk_pixbuf_scale_simple(pixbuf2, 1920, 1080, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(background, pixbuf2);
    
    GtkImage* logo_image = GTK_IMAGE(gtk_builder_get_object(builder, "logo_image"));
    GdkPixbuf* pix = gdk_pixbuf_new_from_file("image_test/Melouande.png", NULL);
    sizing_image(pix, logo_image, 300);
    
    GtkImage* sudoku_image = GTK_IMAGE(gtk_builder_get_object(builder, "sudoku_image"));
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("image_test/Melouande.png", NULL);
    sizing_image(pixbuf, sudoku_image, 800);

    GtkButton* previous_step_button = GTK_BUTTON(gtk_builder_get_object(builder, "previous_step_button"));
    GtkButton* next_step_button = GTK_BUTTON(gtk_builder_get_object(builder, "next_step_button"));
    GtkButton* final_step_button = GTK_BUTTON(gtk_builder_get_object(builder, "final_step_button"));

    GtkButton* quit_button = GTK_BUTTON(gtk_builder_get_object(builder, "quit_button"));
    GtkWidget* download_button = GTK_WIDGET(gtk_builder_get_object(builder, "download_button"));

    GtkButton* solve_button = GTK_BUTTON(gtk_builder_get_object(builder, "solve_button"));


    //gtk_widget_set_size_request(GTK_WIDGET(window), 1080, 1920);

    All all =
    {
        .state1 = HIDDEN, //previous
        .state2 = HIDDEN, //next
        .state3 = HIDDEN, //final
        .state4 = SHOWN, //download

        .sudoku_file = "solved.png",

        .ui = 
        {
            .window = window,
            .fixed = fixed,
            .previous_step_button = previous_step_button,
            .next_step_button = next_step_button,
            .final_step_button = final_step_button,
            .quit_button = quit_button,
            .download_button = download_button,
            .solve_button = solve_button,
            .sudoku_image = sudoku_image,
            .logo_image = logo_image,
            .background = background,
            .title = title,
        },
    };

   
    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(download_button, "selection-changed", G_CALLBACK (file_selected_changed), &all);
    
    g_signal_connect(previous_step_button, "clicked", G_CALLBACK(on_state_psb), &all);
    g_signal_connect(next_step_button, "clicked", G_CALLBACK(on_state_nsb), &all);
    g_signal_connect(final_step_button, "clicked", G_CALLBACK(on_state_fsb), &all);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(begin), &all);
    
    g_signal_connect(quit_button, "clicked", gtk_main_quit, &all);

    gtk_widget_show_all(GTK_WIDGET(window));

    gtk_widget_hide(GTK_WIDGET(previous_step_button));
    gtk_widget_hide(GTK_WIDGET(next_step_button));
    gtk_widget_hide(GTK_WIDGET(final_step_button));
    gtk_widget_hide(GTK_WIDGET(download_button));
    gtk_widget_hide(GTK_WIDGET(logo_image));
    gtk_widget_hide(GTK_WIDGET(sudoku_image));
    gtk_widget_hide(GTK_WIDGET(title));

    
    gtk_main();
    return 0;
}   
