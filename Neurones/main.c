#include <stdio.h>
#include <stdlin.h>
#include <string.h>
#include <err.h>
#include "neuron_utils.h"

void exit_help()
{
    char* help =
        "Usage:\n
        --xor valid_data_path\n
        --digit valid_data_path";

    errx(EXIT_FAILURE, "%s", help);
}

void main(size_t argc, char** argv)
{
    if(argc != 3)
        exit_help();

    double *sizes[3];

    if(argv[1] == "xor")
    {
        sizes[0] = 2;
        sizes[1] = 2;
        sizes[2] = 2;
    }
    else if (argv[1] == "digit")
    {
        sizes[0] = 256;
        sizes[1] = 256;
        size[2] = 10;
    }
    else
        exit_help();

    FILE* file;
    file = fopen(argv[2]);
    if (!file)
        exit_help();

    // TODO:
    // - Lire le fichier
    // - Faire 2 listes, les input et les resultats
    // - Learn sur tous
    // - Garder un echantillion pour tester apres l'entrainement
}
