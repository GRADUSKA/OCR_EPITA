#include <gtk/gtk.h>

#include "../Image_Processing/image_process.h"
#include "../Grid_Detection/detection.h"
#include "../Result_grid/result_grid.h"
#include "../Solver/solver.h"
#include "../Boxes_cutting/boxes.h"

typedef enum Save
{
    T,
    F,
}Save;

typedef enum StateButton
{
    SHOWN,
    HIDDEN,
    SUDOKU,
    GRAYSCALE,
    GAUSSIAN_BLUR,
    CANNY,
    HOUGH,
    ROTATION,
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
    Save is_save;
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
    int h = gdk_pixbuf_get_height(pixbuf);
    int w = gdk_pixbuf_get_width(pixbuf);
    int new_w = size;
    int new_h = size;

    new_w = (w * 800) / h;
    if(new_w > 1000)
    {
        new_h = (new_h * 1000)/new_w;
        new_w = 1000;
    }
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, new_w, new_h, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(image, pixbuf);
}


void set_save3(All* all) // the final step button becomes the save button and next step button disappear
{
    all->state1 = SAVE;
    all->state2 = SAVE;
    all->state3 = SAVE;

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("Solved.bmp", NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);

    gtk_button_set_label(all->ui.final_step_button, "Save");

    gtk_widget_hide(GTK_WIDGET(all->ui.next_step_button));
}


void set_back3(All* all)
{
    //funtion to save the the sudoku solved file
    all->is_save = T;
    remove("Grayscale.bmp");
    remove("Rotation.bmp");
    remove("Gaussian.bmp");
    remove("Canny.bmp");
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

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("Grayscale.bmp", NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);
}


void set_rotation(All* all)
{
    all->state1 = ROTATION;
    all->state2 = ROTATION;

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("Rotation.bmp", NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);
}


void set_gaussian_blur(All* all)
{
    all->state1 = GAUSSIAN_BLUR;
    all->state2 = GAUSSIAN_BLUR;

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("Gaussian.bmp", NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);
}

void set_hough(All* all)
{
    all->state1 = HOUGH;
    all->state2 = HOUGH;

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("Hough.bmp", NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);
}


void set_canny(All* all)
{
    all->state1 = CANNY;
    all->state2 = CANNY;

    gtk_widget_show(GTK_WIDGET(all->ui.next_step_button));
    gtk_button_set_label(all->ui.final_step_button, "Final step");

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("Canny.bmp", NULL);
    sizing_image(pixbuf, all->ui.sudoku_image, 800);
}


void on_state_psb(GtkButton *button, gpointer user_data) //previous step button
{
    All* all = user_data;

    switch(all->state1) // previous
    {
        case SUDOKU: set_shown4(all); break;
        case GRAYSCALE: set_sudoku(all); break;
        case GAUSSIAN_BLUR: set_grayscale(all); break;
        case CANNY: set_gaussian_blur(all); break;
        case HOUGH: set_canny(all); break;
        case ROTATION: set_hough(all); break;
        case SAVE: set_rotation(all); break;
        default: break;
    };
}


void on_state_nsb(GtkButton *button, gpointer user_data) // next step button
{
    All* all = user_data;

    switch(all->state2) // next
    {
        case SUDOKU: set_grayscale(all); break;
        case GRAYSCALE: set_gaussian_blur(all); break;
        case GAUSSIAN_BLUR: set_canny(all); break;
        case CANNY: set_hough(all); break;
        case HOUGH: set_rotation(all); break;
        case ROTATION: set_save3(all); break;
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

    load_the_css("Graphic_Interface/color.css");

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
    sizing_image(pixbuf, all->ui.sudoku_image, 800);
    set_hidden4(all);

    image_process(all->sudoku_file, "--grayscale", "0");
    image_process("Grayscale.bmp", "--gaussian", "0");
    image_process("Gaussian.bmp", "--canny", "0");
    detection("Canny.bmp", "--hough", "0");
    detection("Hough.bmp", "--rotation", "0");
    detection("Rotation.bmp", "--blob", "0");
    detection(all->sudoku_file,"--all","0");
    boxes("Blob.bmp");
    solver1("grid");
    result();
}

void quit(GtkButton *button, gpointer user_data)
{
    All *all = user_data;

    if(all->is_save == F)
    {
        remove("Solved.bmp");
    }
    remove("Grayscale.bmp");
    remove("Rotation.bmp");
    remove("Gaussian.bmp");
    remove("Canny.bmp");
    remove("Hough.bmp");
    remove("Blob.bmp");
    remove("grid");
    remove("grid.result");

    gtk_main_quit();
}

int main (int argc, char *argv[])
{
    gtk_init(NULL, NULL);

    load_the_css("Graphic_Interface/bcolor.css");

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "Graphic_Interface/interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    GtkFixed* fixed = GTK_FIXED(gtk_builder_get_object(builder, "fixed"));

    GtkImage* title = GTK_IMAGE(gtk_builder_get_object(builder, "title"));
    GdkPixbuf* pix2 = gdk_pixbuf_new_from_file("Graphic_Interface/images/title.png", NULL);
    pix2 = gdk_pixbuf_scale_simple(pix2, 700, 130, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(title, pix2);


    GtkImage* background = GTK_IMAGE(gtk_builder_get_object(builder, "background"));
    GdkPixbuf* pixbuf2 = gdk_pixbuf_new_from_file("Graphic_Interface/images/background.png", NULL);
    pixbuf2 = gdk_pixbuf_scale_simple(pixbuf2, 1920, 1080, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(background, pixbuf2);

    GtkImage* logo_image = GTK_IMAGE(gtk_builder_get_object(builder, "logo_image"));
    GdkPixbuf* pix = gdk_pixbuf_new_from_file("Graphic_Interface/images/Melouande.png", NULL);
    pix = gdk_pixbuf_scale_simple(pix, 300, 300, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(logo_image, pix);

    GtkImage* sudoku_image = GTK_IMAGE(gtk_builder_get_object(builder, "sudoku_image"));
    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("Graphic_Interface/images/Melouande.png", NULL);
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

        .sudoku_file = "Graphic_Interface/images/Melouande.png",

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
        .is_save = F,
    };


    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(download_button, "selection-changed", G_CALLBACK (file_selected_changed), &all);

    g_signal_connect(previous_step_button, "clicked", G_CALLBACK(on_state_psb), &all);
    g_signal_connect(next_step_button, "clicked", G_CALLBACK(on_state_nsb), &all);
    g_signal_connect(final_step_button, "clicked", G_CALLBACK(on_state_fsb), &all);
    g_signal_connect(solve_button, "clicked", G_CALLBACK(begin), &all);

    g_signal_connect(quit_button, "clicked", G_CALLBACK(quit), &all);
    //g_signal_connect(quit_button, "clicked", gtk_main_quit, &all);

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
