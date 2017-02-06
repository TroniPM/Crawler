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
#include "parser.h"

//#define PRINT_LINKS_FOUND 1

//FILE* CURRENT_URL_FILE;
char * DOMAIN1;
char * FILENAME;
char * FILENAME_PATH_DOWNLOADED;
char * FILENAME_LINK_WORKSPACE;
char * FILENAME_LINK_FINAL;

FILE* openFile(char * arq) {
    //return fopen("links.txt", "a+");
    //return fopen(str_concat("./workspace_crowler/", "links.txt"), "a+");

    return fopen(arq, "a+");
    //sprintf(path, str_concat(str_concat("./", workspace_main), "%s"), nomeArquivo);
}

void closeFile(FILE* arq) {
    fclose(arq);
}

void writeLinkOnFileWorkSpace(char *txt) {
    //logs("writeLinkOnFile()");
    FILE * arq = openFile(FILENAME_LINK_WORKSPACE);
    if (arq != NULL) {
        int ch = 0;
        int lines = 1;
        while (!feof(arq)) {
            ch = fgetc(arq);
            if (ch == '\n' || ch == '\r') {
                lines++;
            }
        }

        //logi(lines);
        fprintf(arq, "%s\n", str_trim(txt));
    }
    closeFile(arq);
}

void writeLinkOnFileFinal(char *txt) {
    //logs("writeLinkOnFile()");
    FILE * arq = openFile(FILENAME_LINK_FINAL);
    if (arq != NULL) {
        int ch = 0;
        int lines = 1;
        while (!feof(arq)) {
            ch = fgetc(arq);
            if (ch == '\n' || ch == '\r') {
                lines++;
            }
        }

        //logi(lines);
        if (lines <= 9) {
            fprintf(arq, "%d     - %s\n", lines, str_trim(txt));
        } else if (lines <= 99) {
            fprintf(arq, "%d    - %s\n", lines, str_trim(txt));
        } else if (lines <= 999) {
            fprintf(arq, "%d   - %s\n", lines, str_trim(txt));
        } else if (lines <= 9999) {
            fprintf(arq, "%d  - %s\n", lines, str_trim(txt));
        } else {
            fprintf(arq, "%d - %s\n", lines, str_trim(txt));
        }

    }
    closeFile(arq);
}

void writeLinkOnFileOtherDomain(char *txt) {
    //logs("writeLinkOnFile()");
    FILE * arq = openFile("links_otherDomains");
    if (arq != NULL) {
        int ch = 0;
        int lines = 1;
        while (!feof(arq)) {
            ch = fgetc(arq);
            if (ch == '\n' || ch == '\r') {
                lines++;
            }
        }

        //logi(lines);
        if (lines <= 9) {
            fprintf(arq, "%d     - %s\n", lines, str_trim(txt));
        } else if (lines <= 99) {
            fprintf(arq, "%d    - %s\n", lines, str_trim(txt));
        } else if (lines <= 999) {
            fprintf(arq, "%d   - %s\n", lines, str_trim(txt));
        } else if (lines <= 9999) {
            fprintf(arq, "%d  - %s\n", lines, str_trim(txt));
        } else {
            fprintf(arq, "%d - %s\n", lines, str_trim(txt));
        }

    }
    closeFile(arq);
}

void writeLinkOnFileOtherFiles(char *txt) {
    //logs("writeLinkOnFile()");
    FILE * arq = openFile("links_otherFiles.txt");
    if (arq != NULL) {
        int ch = 0;
        int lines = 1;
        while (!feof(arq)) {
            ch = fgetc(arq);
            if (ch == '\n' || ch == '\r') {
                lines++;
            }
        }

        //logi(lines);
        if (lines <= 9) {
            fprintf(arq, "%d     - %s\n", lines, str_trim(txt));
        } else if (lines <= 99) {
            fprintf(arq, "%d    - %s\n", lines, str_trim(txt));
        } else if (lines <= 999) {
            fprintf(arq, "%d   - %s\n", lines, str_trim(txt));
        } else if (lines <= 9999) {
            fprintf(arq, "%d  - %s\n", lines, str_trim(txt));
        } else {
            fprintf(arq, "%d - %s\n", lines, str_trim(txt));
        }

    }
    closeFile(arq);
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
                                str = removeHttpFromLink(str);

                                if (str_endsWith(str, "/"))
                                    str = str_removeLastCharFromString(str);
                                if (PRINT_LINKS_FOUND) {
                                    logs(str_concat("LINK: ", str));
                                }

                                writeLinkOnFileWorkSpace(str);
                                qtd++;
                            } else {//OTHER DOMAIN
                                str = removeHttpFromLink(str);
                                if (str_endsWith(str, "/"))
                                    str = str_removeLastCharFromString(str);
                                writeLinkOnFileOtherDomain(str);
                            }
                        } else {//OTHER FILES
                            str = removeHttpFromLink(str);
                            if (str_endsWith(str, "/"))
                                str = str_removeLastCharFromString(str);
                            writeLinkOnFileOtherFiles(str);
                        }
                    }

                }

                pch = strtok(NULL, " ");
            }
        }
    }
    return qtd;
}

char * removeHttpFromLink(char * str) {
    char * aux = str;
    if (str_startsWith(str_toLowerCase(aux), "http://")) {
        memmove(aux, aux + 7, strlen(aux));
    } else if (str_startsWith(str_toLowerCase(aux), "https://")) {
        memmove(aux, aux + 8, strlen(aux));
    }
    return aux;
}

int parserINIT(char * name, char * path_with_filename, char * url) {
    logs("------------------------------------------------------");
    logs("parseINIT()");
    DOMAIN1 = url;
    FILENAME = name;
    FILENAME_PATH_DOWNLOADED = path_with_filename;
    FILENAME_LINK_WORKSPACE = str_concat(str_concat(str_concat("./", workspace_main), workspance_links), FILENAME);
    //FILENAME_LINK_FINAL = str_concat(str_concat("./", workspace_main), FILENAME);
    FILENAME_LINK_FINAL = "links.txt"; //str_concat(str_concat("./", workspace_main), FILENAME);
    //logs(str_concat("parserINIT() DOMAIN: ", url));
    //logs(str_concat("parserINIT() PATH: ", path_with_filename));
    //logs(str_concat("parserINIT() FILENAME: ", name));

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    /**/
    fp = fopen(FILENAME_PATH_DOWNLOADED, "r");
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

    removeDuplicatedLinks();

    return qntd_links;
}

int removeDuplicatedLinks() {
    char * frase = readfile(FILENAME_LINK_WORKSPACE);


    char** tokens = str_split(frase, '\n');
    int i;
    for (i = 0; *(tokens + i); i++) {
        writeLinkOnFileFinal(*(tokens + i));
        free(*(tokens + i));
    }
    free(tokens);

    return 0;
}

char * getDomainWithOutBar() {
    if (str_endsWith(DOMAIN1, "/")) {
        return str_removeLastCharFromString(DOMAIN1);
    }
    return DOMAIN1;
}

char * getDomainWithBar() {
    if (!str_endsWith(DOMAIN1, "/")) {
        return str_concat(DOMAIN1, "/");
    }
    return DOMAIN1;
}

char * completarLink(char * str) {
    //logs("completarLink()");
    char * aux = addBarraAString(str);
    if (str_startsWith(aux, "http") || str_startsWith(aux, "www")) {
        //dummy if
    } else if (str_startsWith(aux, "/") && !str_startsWith(aux, "//")) {
        aux = str_concat(getDomainWithOutBar(), aux);
    } else if (!str_contains(aux, ".")) {
        aux = str_concat(getDomainWithBar(), aux);
    } else if (str_endsWith(str, ".html") || str_endsWith(str, ".htm")) {
        aux = str_concat(getDomainWithBar(), aux);
    }

    return aux;
}

int checkIfLinkIsSameDomain(char * str) {
    //logs("checkIfLinkIsSameDomain()");
    char * aux = str;
    //logs(str_concat(str, str_concat("\t|||\t", DOMAIN)));

    if (str_contains(str, getDomainWithOutBar())) {
        return 1;
    } else {
        //logs("NÃO EH MESMO DOMINIO");
        return 0;
    }
}