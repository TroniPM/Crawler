/******************************************************************************
 * FILE: parser.c
 * DESCRIPTION:
 *   Crowler to map whole website (webpages, imagens, style files, etc).
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex.h>
#include <ctype.h>
#include <dirent.h>

#include "methods.h"
#include "stringmethods.h"
#include "settings.h"
#include "parser.h"

char * DOMAIN1;
char * FILENAME;
char * FILENAME_PATH_DOWNLOADED;
char * FILENAME_LINK_WORKSPACE;
char * FILENAME_LINK_PRE_FINAL;

void writeLinkOnFileNotDownloaded(char * motivo, char * link);

FILE* openFile(char * arq) {
    return fopen(arq, "a+");
}

void closeFile(FILE* arq) {
    fclose(arq);
}

void writeLinkOnFileNotDownloaded(char * motivo, char * link) {
    char * workPath = str_concat(str_concat("./", workspace_main), FILENAME_LINKS_NOT_DOWNLOADED);
    //logs(str_concat("writeLinkOnFileNotDownloaded() ", workPath));
    FILE * arq = openFile(workPath);
    //char * full = readfile(workPath);
    if (arq != NULL) {
        fprintf(arq, "MOTIVO: %s\nLINK: %s\n", motivo, link);
    }
    closeFile(arq);
}

int checkIfLinkWasDownloaded(char * link) {
    //    if (str_contains(link, "index.html"))
    //        printf("checkIfLinkWasDownloaded() %s\n", link);
    char * workPath = str_concat(str_concat("./", workspace_main), FILENAME_LINKS_DOWNLOADED);
    char * full = readfile(workPath);

    if (full != NULL) {
        char** linhasArr = str_split(full, '\n');
        int i;
        for (i = 0; *(linhasArr + i); i++) {
            //            if (str_contains(link, "index.html"))
            //                printf("--------> %s == %s\n", link, *(linhasArr + i));
            if (str_equals(link, *(linhasArr + i))) {
                //                if (str_contains(link, "index.html"))
                //                    printf("RETORNANDO 1 PORQUE ENCONTROU\n");
                return 1;
            }
        }
        free(linhasArr);
    }

    return 0;
}

void writeLinkOnFileDownloaded(char *txt) {
    //logs(str_concat("writeLinkOnFileDownloaded() ", txt));
    //logs("writeLinkOnFile()");
    char * workPath1 = str_concat(str_concat("./", workspace_main), FILENAME_LINKS_DOWNLOADED);
    FILE * arq = openFile(workPath1);

    if (arq != NULL) {
        fprintf(arq, "%s\n", txt);
    }
    closeFile(arq);
}

void writeAndEnumerate(char * link) {
    //logs(str_concat("writeAndEnumerate() INICIO ", link));
    char * linhasFinal = readfile(FILENAME_LINKS);

    if (linhasFinal == NULL) {
        FILE * arq = openFile(FILENAME_LINKS);
        if (arq != NULL) {
            fprintf(arq, "1 -> %s\n", (link));
            closeFile(arq);
        }
    }

    char** arr = str_split(linhasFinal, '\n');

    if (arr == NULL || !*(arr)) {
        FILE * arq = openFile(FILENAME_LINKS);
        if (arq != NULL) {
            fprintf(arq, "1 -> %s\n", (link));
            closeFile(arq);
        }
    }

    if (linhasFinal != NULL && arr != NULL && *(arr)) {
        int j, boolean = 1;
        for (j = 0; *(arr + j); j++) {
            size_t tamanhoArr;
            //char ** linha = str_split(*(arr + j), '-');
            char ** linha = split(*(arr + j), " -> ", &tamanhoArr);
            if (linha != NULL && linha[1] != NULL) {
                //if (linha != NULL && *(linha + 1)) {
                //printf("%s == %s\n", link, str_trim(*(linha + 1)));
                if (str_equals(link, str_trim(*(linha + 1)))) {
                    boolean = 0;
                }
            }
        }
        if (boolean) {
            FILE * arq = openFile(FILENAME_LINKS);
            if (arq != NULL) {
                fprintf(arq, "%d -> %s\n", (j + 1), str_trim(link));
                closeFile(arq);
            }
        }
    }
}

void writeEmailOnFile(char * link) {
    //logs(str_concat("writeEmailOnFile() INICIO ", link));
    char * linhasFinal = readfile(FILENAME_EMAIL);

    if (linhasFinal == NULL) {
        FILE * arq = openFile(FILENAME_EMAIL);
        if (arq != NULL) {
            fprintf(arq, "1 -> %s\n", (link));
            closeFile(arq);
        }
    }

    char** arr = str_split(linhasFinal, '\n');

    if (arr == NULL || !*(arr)) {
        FILE * arq = openFile(FILENAME_EMAIL);
        if (arq != NULL) {
            fprintf(arq, "1 -> %s\n", (link));
            closeFile(arq);
        }
    }

    if (linhasFinal != NULL && arr != NULL && *(arr)) {
        int j, boolean = 1;
        for (j = 0; *(arr + j); j++) {
            size_t tamanhoArr;
            //char ** linha = str_split(*(arr + j), '-');
            char ** linha = split(*(arr + j), " -> ", &tamanhoArr);
            if (linha != NULL && linha[1] != NULL) {
                //if (linha != NULL && *(linha + 1)) {
                //printf("%s == %s\n", link, str_trim(*(linha + 1)));
                if (str_equals(link, str_trim(*(linha + 1)))) {
                    boolean = 0;
                }
            }
        }
        if (boolean) {
            FILE * arq = openFile(FILENAME_EMAIL);
            if (arq != NULL) {
                fprintf(arq, "%d -> %s\n", (j + 1), str_trim(link));
                closeFile(arq);
            }
        }
    }
}

void writeLinkOnFileWorkSpace(char *txt) {
    //logs("writeLinkOnFileWorkSpace()");
    //FILE * arq = openFile(FILENAME_LINK_WORKSPACE);
    FILE * arq = openFile(FILENAME_LINK_WORKSPACE);
    if (arq != NULL) {
        fprintf(arq, "%s\n", str_trim(txt));

        //        char * string = readfile(FILENAME_LINK_WORKSPACE);
        //        char ** arr = str_split(string, '\n');
        //        int i;
        //        for (i = 0; *(arr + i); i++) {
        //
        //        }
        //
        //        fprintf(arq, "%d -> %s\n", (i + 1), str_trim(txt));
    }
    closeFile(arq);
}

void writeLinkOnFileFinal(char *txt) {
    //logs("writeLinkOnFile()");
    FILE * arq = openFile(FILENAME_LINK_PRE_FINAL);
    if (arq != NULL) {
        int ch = 0;
        int lines = 1;
        while (!feof(arq)) {
            ch = fgetc(arq);
            if (ch == '\n' || ch == '\r') {
                lines++;
            }
        }


        fprintf(arq, "%s\n", str_trim(txt));

        /*if (lines <= 9) {
            fprintf(arq, "%d     - %s\n", lines, str_trim(txt));
        } else if (lines <= 99) {
            fprintf(arq, "%d    - %s\n", lines, str_trim(txt));
        } else if (lines <= 999) {
            fprintf(arq, "%d   - %s\n", lines, str_trim(txt));
        } else if (lines <= 9999) {
            fprintf(arq, "%d  - %s\n", lines, str_trim(txt));
        } else {
            fprintf(arq, "%d - %s\n", lines, str_trim(txt));
        }*/

    }
    closeFile(arq);
}

void writeLinkOnFileOtherDomain(char *txt) {
    //logs("writeLinkOnFile()");
    FILE * arq = openFile(FILENAME_OTHERDOMAINS);
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
    FILE * arq = openFile(FILENAME_OTHERFILES);
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

int checkIfLinkHasAnchor(char * str) {
    //logs("checkIfLinkHasAnchor");
    if (str_contains(str, "#")) {
        char** arr = str_split(str, '#');
        int /*tam = (sizeof (getExtensionsAllowed()) / sizeof (char *)),*/ j, i;
        char ** extensoes = getExtensionsAllowed();
        for (i = 0; *(arr + i); i++) {
            for (j = 0; j < getExtensionsAllowedSize(); j++) {
                if (str_endsWith(*(arr + i), *(extensoes + j))) {
                    return 1;
                }
            }
        }
        return 0;
    } else {
        return 0;
    }
}

int checkIfLinkIsEmail(char * link) {
    if (str_startsWith(link, "mailto:")) {
        return 1;
    } else {
        return 0;
    }
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
                    //logs(pch);
                    char * str = tratarLink(pch);

                    //Não salvo se link for VAZIO
                    if (strlen(str) != 0) {
                        if (!checkIfLinkIsEmail(str)) {
                            //DESnegar linha para salvar links proibidos (jpg, png, js, css....)
                            if (!checkIfStringHasForbiddenEnding(str)) {
                                str = completarLink(str);

                                //NEGAR linha para salvar links de outros domínios
                                if (checkIfLinkIsSameDomain(str)) {

                                    if (!checkIfLinkHasAnchor(str)) {
                                        str = removeHttpFromLink(str);

                                        if (str_endsWith(str, "/"))
                                            str = str_removeLastCharFromString(str);
                                        if (PRINT_LINKS_FOUND) {
                                            logs(str_concat("LINK: ", str));
                                        }

                                        writeLinkOnFileWorkSpace(str);
                                        writeAndEnumerate(str);
                                        qtd++;
                                    } else {
                                        writeLinkOnFileNotDownloaded("LINK É POSSUI UMA ÂNCORA", str);
                                    }
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
                        } else {//IS EMAIL
                            char * email = str;
                            memmove(email, email + 7, strlen(email)); //removo "mailto:"
                            writeEmailOnFile(email);
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
                            str = tratarSrc(str);

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
    //logs(str_concat("tratarLink() ", link));
    char * aux = link; //str_replace(" ", "", link);

    //Removendo (href=) e (data-href=) da linha
    if (str_startsWith(aux, "data-href=")) {
        memmove(aux, aux + 10, strlen(aux));
    } else if (str_startsWith(aux, "href=")) {
        memmove(aux, aux + 5, strlen(aux));
    }

    //
    //removo aspas iniciais (as vezes por algum motivo vem uma letra no primeiro char...)
    if (!str_startsWith(aux, "a") || !str_startsWith(aux, "b") || !str_startsWith(aux, "c")
            || !str_startsWith(aux, "d") || !str_startsWith(aux, "e") || !str_startsWith(aux, "f")
            || !str_startsWith(aux, "g") || !str_startsWith(aux, "h") || !str_startsWith(aux, "i")
            || !str_startsWith(aux, "j") || !str_startsWith(aux, "k") || !str_startsWith(aux, "l")
            || !str_startsWith(aux, "m") || !str_startsWith(aux, "n") || !str_startsWith(aux, "o")
            || !str_startsWith(aux, "p") || !str_startsWith(aux, "q") || !str_startsWith(aux, "r")
            || !str_startsWith(aux, "s") || !str_startsWith(aux, "t") || !str_startsWith(aux, "u")
            || !str_startsWith(aux, "v") || !str_startsWith(aux, "x") || !str_startsWith(aux, "z")
            || !str_startsWith(aux, "w") || !str_startsWith(aux, "y") || !str_startsWith(aux, "A")
            || !str_startsWith(aux, "B") || !str_startsWith(aux, "C") || !str_startsWith(aux, "D")
            || !str_startsWith(aux, "E") || !str_startsWith(aux, "F") || !str_startsWith(aux, "G")
            || !str_startsWith(aux, "H") || !str_startsWith(aux, "I") || !str_startsWith(aux, "J")
            || !str_startsWith(aux, "K") || !str_startsWith(aux, "L") || !str_startsWith(aux, "M")
            || !str_startsWith(aux, "N") || !str_startsWith(aux, "O") || !str_startsWith(aux, "P")
            || !str_startsWith(aux, "Q") || !str_startsWith(aux, "R") || !str_startsWith(aux, "S")
            || !str_startsWith(aux, "T") || !str_startsWith(aux, "U") || !str_startsWith(aux, "V")
            || !str_startsWith(aux, "X") || !str_startsWith(aux, "Z") || !str_startsWith(aux, "W")
            || !str_startsWith(aux, "Y")) {
        aux = str_removeFirstCharFromString(aux);
    }
    //removo aspas finais
    //aux = str_removeLastCharFromString(aux);

    /*As vezes há .css que possuem algo tipo: 
      ...href="a.css"/>
     O > foi cortado pelo split, antes de entrar nesse método, então fica
      sobrando a " (já q a barra foi cortado pelo str_removeLastCharFromString acima
     */
    if (aux[strlen(aux) - 1] == '/') {
        aux = str_removeLastCharFromString(aux);
    }
    if (aux[strlen(aux) - 1] == '"') {
        aux = str_removeLastCharFromString(aux);
    }
    if (aux[strlen(aux) - 1] == '\'') {
        aux = str_removeLastCharFromString(aux);
    }

    //logs(aux);
    return aux;
}

char * tratarSrc(char* link) {
    //logs(str_concat("tratarSrc() ", link));
    char * aux = link; //str_replace(" ", "", link);

    //Removendo (href=) e (data-href=) da linha
    if (str_startsWith(aux, "data-src=")) {
        memmove(aux, aux + 9, strlen(aux));
    } else if (str_startsWith(aux, "src=")) {
        memmove(aux, aux + 4, strlen(aux));
    }

    //
    //removo aspas iniciais
    aux = str_removeFirstCharFromString(aux);
    //removo aspas finais
    aux = str_removeLastCharFromString(aux);
    if (str_endsWith(aux, "\"")) {
        aux = str_removeLastCharFromString(aux);
    }

    return aux;
}

int checkIfStringHasForbiddenEnding(char* str) {
    //logs("checkIfStringHasForbiddenEnding");
    if (str_startsWith(str, "mailto:")) {
        return 1;
    }

    char ** tokens = str_split(str, '?'); //As vezes existem parameters ao final do link

    char ** extensoes = getExtensionsProhibited();
    int i, j;
    for (i = 0; *(tokens + i); i++) {

        if (str_equals(*(tokens + i), "#")) {
            return 1;
        }

        for (j = 0; j < getExtensionsProhibitedSize(); j++) {
            //printf("%s == %s\n");
            if (str_endsWith(*(tokens + i), *(extensoes + j))) {
                return 1;
            }
        }

        /*Se chegar até aqui, significa que não encontrou extensões proibidas.
        Então se não encontrar as extensões PERMITIDAS, será bloqueado.*/
        if (EXPLICIT) {

            for (j = 0; j < getExtensionsAllowedSize(); j++) {
                if (str_endsWith(*(tokens + i), getExtensionsAllowed()[j])) {
                    return 0;
                }
            }
            /*Se chegar aqui, link não tem extensão permitida, e pode ser um "folder"
            Então irá descartar.*/
            return 1;
        }
    }
    return 0;

}

char * removeHttpFromLink(char * str) {
    char * aux = str;
    if (str_startsWith((aux), "http://")) {
        memmove(aux, aux + 7, strlen(aux));
    } else if (str_startsWith((aux), "https://")) {
        memmove(aux, aux + 8, strlen(aux));
    } else if (str_contains(aux, "://")) {
        size_t tam;
        char ** arr = split(aux, "://", &tam);
        if (arr[1] != NULL) {
            return arr[1];
        }
    }
    return aux;
}

char * parserINIT(char * name, char * path_with_filename, char * url) {
    /*int tam = (sizeof (EXTENSIONS_PROHIBITED) / sizeof (char *)), j;

    for (j = 0; j < tam; j++) {
        logs(EXTENSIONS_PROHIBITED[j]);
    }*/

    //logs("------------------------------------------------------");
    //logs(str_concat("parseINIT(): ", url));
    DOMAIN1 = removeHttpFromLink(url);
    FILENAME = name;
    FILENAME_PATH_DOWNLOADED = path_with_filename;
    FILENAME_LINK_WORKSPACE = str_concat(str_concat(str_concat("./", workspace_main), workspance_links), FILENAME);

    char * path = str_concat("./", str_concat(workspace_main, "links.txt"));

    FILENAME_LINK_PRE_FINAL = path; //str_concat(str_concat("./", workspace_main), FILENAME);
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
    //logs(str_concat("DOMINIO ", getDomainWithBar()));
    int qntd_links = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        if (checkIfLineContainsLink(line) || checkIfLineContainsSrc(line)) {
            qntd_links += tratarLinha(line);
        }
    }

    fclose(fp);
    if (line)
        free(line);
    //removeDuplicatedLinks();

    return FILENAME_LINK_WORKSPACE;
}

void enumerateAndSave() {
    FILE * arq = openFile(FILENAME_LINKS);

    char * pathAndName = str_concat("./", str_concat(workspace_main, "links.txt"));
    char * linhas = readfile(pathAndName);
    if (arq != NULL && linhas != NULL) {
        char** linhasArr = str_split(linhas, '\n');
        int i;
        for (i = 0; *(linhasArr + i); i++) {
            if ((i + 1) <= 9) {
                fprintf(arq, "%d     - %s\n", (i + 1), *(linhasArr + i));
            } else if ((i + 1) <= 99) {
                fprintf(arq, "%d    - %s\n", (i + 1), *(linhasArr + i));
            } else if ((i + 1) <= 999) {
                fprintf(arq, "%d   - %s\n", (i + 1), *(linhasArr + i));
            } else if ((i + 1) <= 9999) {
                fprintf(arq, "%d  - %s\n", (i + 1), *(linhasArr + i));
            } else {
                fprintf(arq, "%d - %s\n", (i + 1), *(linhasArr + i));
            }
        }
        free(linhasArr);
    }

    closeFile(arq);
    free(linhas);
}

void removeDuplicatedLinks() {

    char * linhas = readfile(FILENAME_LINK_WORKSPACE);
    if (linhas == NULL)
        return;

    int qtd = 0;

    /*APAGO DADOS ANTERIORES*/
    FILE * a = fopen(FILENAME_LINK_WORKSPACE, "w");
    if (a != NULL)
        fclose(a);
    /**/

    char** linhasArr = str_split(linhas, '\n');
    int i, j;
    for (i = 0; *(linhasArr + i); i++) {
        char * linhasDownloaded = readfile(FILENAME_LINK_WORKSPACE);
        //Se não houver linha ainda, adiciono
        if (linhasDownloaded == NULL) {
            qtd++;
            writeLinkOnFileWorkSpace(*(linhasArr + i));
            continue;

        }
        char** linhasDownArr = str_split(linhasDownloaded, '\n');
        int j, trigger = 1;
        for (j = 0; *(linhasDownArr + j); j++) {
            if (str_equals(*(linhasArr + i), *(linhasDownArr + j))) {
                trigger = 0;
            }
        }
        if (trigger) {
            qtd++;
            writeLinkOnFileWorkSpace(*(linhasArr + i));
        }
    }
}

void removeDuplicatedLinksFolder() {
    //logs("------------------------------------------------------");
    //logs("removeDuplicatedLinks():");

    DIR *dir;
    struct dirent *ent;

    char * path = str_concat("./", str_concat(workspace_main, workspance_links));

    int qntd_links = 0, links_repetidos = 0;

    if ((dir = opendir(path)) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) {
            if (!str_endsWith(ent->d_name, ".")) {
                //NOVO ARQUIVO DE LINKS
                //printf("%s\n", ent->d_name);
                char * nome = str_concat(path, ent->d_name); //path and file name
                //logs(str_concat("ARQUIVO: ", nome));

                char * linhas = readfile(nome);
                if (linhas != NULL) {
                    char** linhasArr = str_split(linhas, '\n');

                    int i;

                    for (i = 0; *(linhasArr + i); i++) {
                        //logs(str_concat("LINK: ", *(linhasArr + i)));
                        qntd_links++;

                        char * linhasFinal = readfile(FILENAME_LINK_PRE_FINAL);
                        if (linhasFinal == NULL) {
                            writeLinkOnFileFinal(*(linhasArr + i));
                            continue;

                        }
                        char** linhasFinalArr = str_split(linhasFinal, '\n');
                        int j, trigger = 1;

                        for (j = 0; *(linhasFinalArr + j); j++) {//trigger ==1 its like a break
                            if (str_equals(*(linhasArr + i), *(linhasFinalArr + j))) {
                                links_repetidos++;
                                trigger = 0;
                            }
                        }
                        if (trigger) {
                            writeLinkOnFileFinal(*(linhasArr + i));
                        }
                    }
                }
            }
        }
        closedir(dir);
    } else {
        logs("ERROR: COULDN'T GET WORKSPACE DIRECTORY");
        /* could not open directory */
        //perror("");
        //return EXIT_FAILURE;
    }
    printf("QUANTIDADE DE LINKS VÁLIDOS ENCONTRADOS: %d\n", qntd_links);
    printf("QUANTIDADE DE LINKS VÁLIDOS REPETIDOS: %d\n", links_repetidos);
    printf("QUANTIDADE FINAL: %d\n", (qntd_links - links_repetidos));

}

char ** getDomainAndLevels() {
    //logs(str_concat("getDomainAndLevels() ", getDomainWithBar()));

    int j, boolean = 0;
    char ** extensoes = getExtensionsAllowed();
    //logs("AEUHAIEUAHIEUAHEIAUEH");
    char * domainNoBar = getDomainWithOutBar();
    for (j = 0; j < getExtensionsAllowedSize(); j++) {
        if (str_endsWith(domainNoBar, *(extensoes + j))) {
            //logs("AAAAE");
            boolean = 1;
            break;
        }
    }
    if (!boolean) {
        extensoes = getExtensionsProhibited();
        for (j = 0; j < getExtensionsProhibitedSize(); j++) {
            if (str_endsWith(domainNoBar, *(extensoes + j))) {
                boolean = 1;
                break;
            }
        }
    }
    /* Quando url tiver o fim sem ser uma EXTENSÃO (folder), adiciono o ultimo ITEM após a barra.
     *  Quando for uma extensão, desconsidero a ultima. Também faço a verificação
     *  para os ext proibidos. não importa a extensão, o que importa é saber
     *  que É uma extensão.*/
    char * aux = "";

    char **tokens;
    size_t numtokens, i3;
    tokens = split(getDomainWithBar(), "/", &numtokens);
    //logi(numtokens);

    for (i3 = 0; i3 < numtokens; i3++) {
        //logs(tokens[i3]);
        aux = str_concat(aux, str_concat(tokens[i3], "/"));
        if (boolean && i3 + 2 >= numtokens) {
            break;
        }
    }
    //logs(str_concat("getDomainAndLevels() FINAL \t", aux));
    return str_split(aux, '/');
}

char * removerNiveisDiferentes(char * txt) {
    //logs(str_concat("removerNiveisDiferentes() ", txt));
    if (!str_contains(txt, "../")) {
        return txt;
    }

    char * aux = "";
    char * auxN = txt; // = txt;
    char **tokens;
    size_t numtokens;

    int qntd = str_countOccurrences(txt, "../"), x;
    tokens = split(auxN, "/", &numtokens);

    int x1 = 4, xS = 2, xAux;
    for (xAux = 1; xAux < qntd; xAux++) {
        x1 += xS;
    }

    if ((int) numtokens >= x1) {
        for (x = 0; x < qntd; x++) {
            tokens = split(auxN, "/", &numtokens);
            aux = "";
            int ctrl = 1, xx = (int) numtokens, i;

            for (i = 0; i < xx; i++) {
                if (i + 1 == xx) {
                    //dummy if
                    aux = str_concat(aux, tokens [i]);
                } else if (str_equals(tokens[i + 1], "..") && ctrl) {
                    ctrl = 0;
                    i++;
                    continue;

                } else {
                    aux = str_concat(aux, str_concat(tokens [i], "/"));
                }
            }
            auxN = aux;
        }
    } else {
        char * workPath1 = str_concat(str_concat("./", workspace_main), "convertedLinks.txt");
        FILE * arq = fopen(workPath1, "a+");
        if (arq != NULL) {
            fprintf(arq, "INICIAL: %s\tFINAL: %s\n", txt, aux);
            fclose(arq);
        }

        return txt;
    }
    char * workPath1 = str_concat(str_concat("./", workspace_main), "convertedLinks.txt");
    FILE * arq = fopen(workPath1, "a+");
    if (arq != NULL) {
        fprintf(arq, "INICIAL: %s\tFINAL: %s\n", txt, aux);
        fclose(arq);
    }

    return aux;
}

char * getDomain() {
    //logs("getDomain()");
    char * dom = "";

    char **tokens;
    size_t numtokens;
    tokens = split(getDomainWithBar(), "/", &numtokens);
    for (size_t i = 0; i < numtokens; i++) {
        dom = str_concat(dom, tokens[i]);
        break;
    }

    return dom;
}

char * getDomainWithOutBar() {
    if (str_endsWith(DOMAIN1, "/")) {
        char * aux = DOMAIN1;
        memmove(aux, aux, strlen(aux) - 2);
        return aux; //str_removeLastCharFromString(DOMAIN1);
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
    //logs(str_concat("completarLink() INICIO \t", str));
    char * aux = addBarraAString(str);
    if (str_startsWith(aux, "http")
            || str_startsWith(aux, "www")
            || str_startsWith(aux, "ftp")
            || str_startsWith(aux, "mailto")
            || str_startsWith(aux, "//")
            || str_contains(aux, "://")) {
        //dummy if
    } else if (str_startsWith(aux, "/")) {
        //logs(str_concat("completarLink(1) ", str));
        /*exemplo "/painel", isso quer dizer q vai direcionar pra raiz/painel*/
        aux = str_concat(str_concat(getDomain(), "/"), aux);
    } else if (!str_contains(aux, ".")) {
        //logs(str_concat("completarLink(2) ", str));
        /*exemplo "painel/", isso quer dizer q vai direcionar pra raiz/.../painel*/
        char ** folders = getDomainAndLevels();
        if (folders == NULL) {
            aux = str_concat(getDomainWithBar(), aux);
        } else {
            char * full = "";
            int i;
            for (i = 0; *(folders + i); i++) {
                full = str_concat(full, str_concat(*(folders + i), "/"));
            }
            aux = str_concat(full, aux);
        }
    } else {
        char ** folders = getDomainAndLevels();
        if (folders == NULL) {
            //logs("folders==NULL");
            aux = str_concat(getDomainWithBar(), aux);
        } else {
            //logs("folders!=NULL");
            char * full = "";
            int i;
            for (i = 0; *(folders + i); i++) {
                //logs(*(folders + i));

                full = str_concat(full, str_concat(*(folders + i), "/"));
            }

            aux = str_concat(full, aux);
        }
    }
    aux = removerNiveisDiferentes(aux);
    //logs(str_concat("completarLink() FIM\t", aux));

    return aux;
}

int checkIfLinkIsSameDomain(char * str) {
    //logs("checkIfLinkIsSameDomain()");
    char * aux = str;
    //logs(str_concat(str, str_concat("\t|||\t", DOMAIN)));

    if (str_contains(str, getDomain())) {
        return 1;
    } else {
        return 0;
    }
}