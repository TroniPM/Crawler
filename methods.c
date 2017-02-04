/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include "methods.h"

#define ISDEBUG 1

void logs(const char*txt) {
    if (ISDEBUG == 1) {
        printf("LOG: %s\n", txt);
    }
}

void logi(const int txt) {
    if (ISDEBUG == 1) {
        printf("LOG: %d\n", txt);
    }
}

