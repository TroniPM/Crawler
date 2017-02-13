/******************************************************************************
 * FILE: methods.c
 * DESCRIPTION:
 *   Crowler to map whole website (webpages, imagens, style files, etc).
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "settings.h"

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

