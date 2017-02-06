/******************************************************************************
 * FILE: methods.c
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 * LAST REVISED: 04/fev/17
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
//#include "methods.h"
#include "settings.h"

#define ISDEBUG 1



void logs(const char* txt) {
    if (ISDEBUG == 1) {
        printf("LOG: %s\n", txt);
    }
}

void logi(const int txt) {
    if (ISDEBUG == 1) {
        printf("LOG: %d\n", txt);
    }
}

void logc(const char txt) {
    if (ISDEBUG == 1) {
        printf("LOG: %c\n", txt);
    }
}

