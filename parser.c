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
#include "settings.h"

//#define PRINT_LINKS_FOUND 1

FILE* CURRENT_URL_FILE;
char * FILENAME;
char * PATH;
//char * DOMAIN;

void openLinkFile() {
    CURRENT_URL_FILE = fopen("links.txt", "a+");
    //CURRENT_URL_FILE = fopen(FILENAME, "a+");
}

void closeLinkFile() {
    fclose(CURRENT_URL_FILE);
}

void writeLinkOnFile(char *txt) {
    //logs("writeLinkOnFile()");
    openLinkFile();
    if (CURRENT_URL_FILE != NULL) {
        int ch = 0;
        int lines = 1;
        while (!feof(CURRENT_URL_FILE)) {
            ch = fgetc(CURRENT_URL_FILE);
            if (ch == '\n' || ch == '\r') {
                lines++;
            }
        }

        //logi(lines);
        fprintf(CURRENT_URL_FILE, "%d - %s\n", lines, str_trim(txt));
    }
    closeLinkFile();
}

int tratarLinha(char * linha) {
    //logs("tratarLinha()");
    //int qtd = str_countOccurrences(linha, " href=");//qntd de links na mesma linha
    int qtd = 0;
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
                    char * str = tratarLink(pch);

                    //Não salvo se link for VAZIO
                    if (strlen(str) != 0) {
                        //DESnegar linha para salvar links proibidos (jpg, png, js, css....)
                        if (!checkIfStringHasForbiddenEnding(str)) {
                            str = completarLink(str);

                            //NEGAR linha para salvar links de outros domínios
                            if (checkIfLinkIsSameDomain(str)) {
                                if (str_endsWith(str, "/"))
                                    str = str_removeLastCharFromString(str);
                                if (PRINT_LINKS_FOUND) {
                                    logs(str_concat("LINK: ", str));
                                }
                                writeLinkOnFile(str);
                                qtd++;
                            }
                        }
                    }

                }

                pch = strtok(NULL, " ");
            }
        }
    }
    return qtd;
}

int parserINIT(char * file_name) {

    //DOMAIN = url;
    FILENAME = file_name;
    logs(str_concat("parserINIT() ", getDomainWithOutBar()));

    //stringmethods_init(URL_DOMAIN/*, /* EXTENSIONS_PROHIBITED/*, EXTENSIONS_ALLOWED*/);

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    /**/
    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        logs("parserINIT() ERROR AT FILE .HTML/.HTM OPENING");
        exit(EXIT_FAILURE);
    }

    int qntd_links = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        if (checkIfLineContainsLink(line)) {
            qntd_links += tratarLinha(line);
        }
    }

    fclose(fp);
    if (line)
        free(line);

    return qntd_links;
}