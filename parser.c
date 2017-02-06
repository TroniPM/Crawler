/******************************************************************************
 * FILE: parser.c
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 * LAST REVISED: 04/fev/17
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex.h>
#include <ctype.h>

#include "methods.h"
#include "stringmethods.h"
#include "settings.h"
#include "parser.h"

char * DOMAIN1;
char * FILENAME;
char * FILENAME_PATH_DOWNLOADED;
char * FILENAME_LINK_WORKSPACE;
char * FILENAME_LINK_FINAL;

char * EXTENSIONS_PROHIBITED[] = {".rb", ".rhtml", ".dll", ".cfm", ".cgi", ".svg", ".py", "jhtml", ".xhtml", ".swf", ".asp", ".aspx", ".css", ".js", ".xml", ".ico", ".jpg", ".jpeg", ".png", ".csp", ".do", ".jsf", ".jspx", ".php", ".gif"};
char * EXTENSIONS_ALLOWED[] = {".html", ".htm"};

FILE* openFile(char * arq) {
    return fopen(arq, "a+");
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
    FILE * arq = openFile("links_otherDomains.txt");
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
                                if (SAVE_LINKS_OTHERDOMAINS) {
                                    str = removeHttpFromLink(str);
                                    if (str_endsWith(str, "/"))
                                        str = str_removeLastCharFromString(str);
                                    writeLinkOnFileOtherDomain(str);
                                }
                            }
                        } else {//OTHER FILES
                            if (SAVE_LINKS_OTHERFILES) {
                                if (!str_equals(str, "#")) {
                                    str = removeHttpFromLink(str);
                                    if (str_endsWith(str, "/"))
                                        str = str_removeLastCharFromString(str);
                                    writeLinkOnFileOtherFiles(str);
                                }
                            }
                        }
                    }

                }

                pch = strtok(NULL, " ");
            }
        }
        if (checkIfLineContainsSrc(token)) {
            tokenTrimmed = str_trim(token);
            char * pch;

            pch = strtok(tokenTrimmed, " "); //" ,.-"
            //logs(str_concat("!checkIfLineContainsLink() ", pch));
            while (pch != NULL) {
                if (checkIfLineContainsSrc(pch)) {
                    char * str = tratarSrc(pch);

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
                                if (SAVE_LINKS_OTHERDOMAINS) {
                                    str = removeHttpFromLink(str);
                                    if (str_endsWith(str, "/"))
                                        str = str_removeLastCharFromString(str);
                                    writeLinkOnFileOtherDomain(str);
                                }
                            }
                        } else {//OTHER FILES
                            if (SAVE_LINKS_OTHERFILES) {
                                if (!str_equals(str, "#")) {
                                    str = removeHttpFromLink(str);
                                    if (str_endsWith(str, "/"))
                                        str = str_removeLastCharFromString(str);
                                    writeLinkOnFileOtherFiles(str);
                                }
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

int checkIfLineContainsSrc(char * line) {
    //logs("checkIfLineContainsLink()");
    regex_t regex;
    int reti;
    char msgbuf[100];
    int result;
    /**/
    /* Compile regular expression */
    reti = regcomp(&regex, "src=", 0);
    if (reti) {
        logs("Could not compile regex");
        return 0;
    }

    reti = regexec(&regex, line, 0, NULL, 0);
    if (!reti) {
        result = 1; //Is link
    } else if (reti == REG_NOMATCH) {
        result = 0; //No link
    }

    regfree(&regex);
    return result;
}

/*
 return 1 if has link
 return 0 if hasn't link
 */
int checkIfLineContainsLink(char * line) {
    //logs("checkIfLineContainsLink()");
    regex_t regex;
    int reti;
    char msgbuf[100];
    int result;
    /**/
    /* Compile regular expression */
    reti = regcomp(&regex, "href=", 0);
    if (reti) {
        logs("Could not compile regex");
        return 0;
    }

    reti = regexec(&regex, line, 0, NULL, 0);
    if (!reti) {
        result = 1; //Is link
    } else if (reti == REG_NOMATCH) {
        result = 0; //No link
    }

    regfree(&regex);
    return result;
}

char * tratarLink(char* link) {
    //logs("tratarLink()");
    char * aux = link; //str_replace(" ", "", link);

    //Removendo (href=) e (data-href=) da linha
    if (str_startsWith(aux, "data-href=")) {
        memmove(aux, aux + 10, strlen(aux));
    } else if (str_startsWith(aux, "href=")) {
        memmove(aux, aux + 5, strlen(aux));
    }

    //
    //removo aspas iniciais
    aux = str_removeFirstCharFromString(aux);
    //removo aspas finais
    aux = str_removeLastCharFromString(aux);

    //char * aux1;
    //logc(aux[strlen(aux) - 1]);
    //    if (strlen(aux) > 0) {
    //        if (strlen(aux) == 1 && aux[0] == '#' && aux[0] == '/') {
    //            //dummy if
    //        } else {
    //            //aux = addBarraAString(aux);
    //        }
    //    }
    return aux;
}

char * tratarSrc(char* link) {
    //logs("tratarLink()");
    char * aux = link; //str_replace(" ", "", link);

    //Removendo (href=) e (data-href=) da linha
    if (str_startsWith(aux, "src=")) {
        memmove(aux, aux + 4, strlen(aux));
    }

    //
    //removo aspas iniciais
    aux = str_removeFirstCharFromString(aux);
    //removo aspas finais
    aux = str_removeLastCharFromString(aux);

    return aux;
}

int checkIfStringHasForbiddenEnding(char* str) {
    char ** tokens = str_split(str, '?'); //As vezes existem parameters ao final do link
    int i;
    for (i = 0; *(tokens + i); i++) {

        if (str_equals(*(tokens + i), "#")) {
            return 1;
        }
        int j = 0;
        int tam = (sizeof (EXTENSIONS_PROHIBITED) / sizeof (char *));

        for (j = 0; j < tam; j++) {
            //logs(str_concat(str_concat(*(tokens + i), "\t==\t"), EXTENSIONS_PROHIBITED[j]));
            if (str_endsWith(*(tokens + i), EXTENSIONS_PROHIBITED[j])
                    /*|| str_endsWith(*(tokens + i), ".js")
                    || str_endsWith(*(tokens + i), ".xml")
                    || str_endsWith(*(tokens + i), ".ico")
                    || str_endsWith(*(tokens + i), ".jpg")
                    || str_endsWith(*(tokens + i), ".png")
                    || str_endsWith(*(tokens + i), ".csp")
                    || str_endsWith(*(tokens + i), ".do")
                    || str_endsWith(*(tokens + i), ".jsf")
                    || str_endsWith(*(tokens + i), ".php")*/) {//Verifico se linha é igual a # (ancora)
                return 1;
            }
        }
    }
    return 0;

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
    /*int tam = (sizeof (EXTENSIONS_PROHIBITED) / sizeof (char *)), j;

    for (j = 0; j < tam; j++) {
        logs(EXTENSIONS_PROHIBITED[j]);
    }*/

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
        if (checkIfLineContainsLink(line) || checkIfLineContainsSrc(line)) {
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