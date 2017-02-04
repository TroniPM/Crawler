/******************************************************************************
 * FILE: parser.c
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 * LAST REVISED: 04/fev/17
 ******************************************************************************/

#include <string.h>
//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex.h>  
//#include <assert.h>
#include <ctype.h>

#include "methods.h"
#include "stringmethods.h"

#define PRINT_LINKS_FOUND 1

FILE* currentURL;

void openLinkFile() {
    currentURL = fopen("links.txt", "a+");
}

void closeLinkFile() {
    fclose(currentURL);
}

void writeLinkOnFile(char *txt) {
    openLinkFile();
    if (currentURL != NULL) {
        int ch = 0;
        int lines = 1;
        while (!feof(currentURL)) {
            ch = fgetc(currentURL);
            if (ch == '\n' || ch == '\r') {
                lines++;
            }
        }

        //logi(lines);
        fprintf(currentURL, "%d - %s\n", lines, str_trim(txt));
    }
    closeLinkFile();
}

int tratarLinha(char * linha) {
    int qtd = str_countOccurrences(linha, " href=");
    qtd = 0;
    //printf("Quantidade de Links: %d || %s", qtd, linha);

    char *token, *tokenTrimmed, *string, *tofree;
    tofree = string = strdup(linha);

    while ((token = strsep(&string, ">")) != NULL) {
        if (checkIfLineContainsLink(token)) {

            tokenTrimmed = str_trim(token);
            char * pch;

            pch = strtok(tokenTrimmed, " "); //" ,.-"

            while (pch != NULL) {
                if (checkIfLineContainsLink(pch)) {
                    //Removendo href= da linha
                    memmove(pch, pch + 5, strlen(pch));
                    //Veficação de link (ver se é html/htm aqui)
                    if (strcmp(pch, "\"#\"") != 0
                            && strlen(pch) > 2) {
                        if (str_endsWith(pch, ".css\"")
                                || str_endsWith(pch, ".js\"")
                                || str_endsWith(pch, ".xml\"")
                                || str_endsWith(pch, ".ico\"")
                                || str_endsWith(pch, ".php\"")) {
                        } else {
                            if (PRINT_LINKS_FOUND) {
                                logs(str_concat("LINK: ", pch));
                            }
                            writeLinkOnFile(pch);
                            qtd++;
                        }
                    }

                }

                pch = strtok(NULL, " ");
            }
        }
    }
    return qtd;
}

int parserINIT(char * filename, char * url) {
    if (str_endsWith(url, "/"))
        str_removeLastCharFromString(url);
    logs(str_concat("parserINIT() ", url));

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    /**/
    fp = fopen(filename, "r");
    if (fp == NULL) {
        logs("parserINIT() ERROR AT FILE .HTML/.HTM OPENING");
        exit(EXIT_FAILURE);
    }

    int qntd_links = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);
        if (checkIfLineContainsLink(line)) {
            qntd_links += tratarLinha(line);
        }
    }

    fclose(fp);
    if (line)
        free(line);

    return qntd_links;
}