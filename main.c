/******************************************************************************
 * FILE: main.c
 * DESCRIPTION:
 *   Crawler to map whole website (webpages, imagens, style files, etc).
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/

#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "parser.h"
#include "settings.h"
#include "methods.h"
#include "stringmethods.h"

#define NUM_THREADS 15

void repeatScheme(char * txt, int nv);
void writeLinkOnFileNotDownloaded(char * motivo, char * link);
void printHelp();
void forkCreated();
void writeAndEnumerate(char * link);

int masterPID = -1;

struct ObjetoThread {
    char* url;
    long threadid;
};

void *newUrlScheme(void * parametro) {
    struct ObjetoThread* obj = parametro;
    long tid = (long) obj->threadid;
    char * url = (char *) obj->url;
    //printf("Do stuff. Thread id: #%ld, URL: %s:%ld!\n", tid, url, tid);
    int i;
    for (i = 0; i < 5; i++) {
        printf("Do stuff. Thread id: #%ld, URL: %s:%ld!\t%d\n", tid, url, tid, i);
    }
    pthread_exit(NULL);
}

int createThread() {

    /**/

    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    char * txt = "www.jpcontabil.com";
    for (t = 0; t < NUM_THREADS; t++) {
        /**/
        struct ObjetoThread obj;
        obj.url = "www.google.com";
        obj.threadid = t;
        /**/
        printf("main() creating thread: #%ld\n", t);
        rc = pthread_create(&threads[t], NULL, newUrlScheme, &obj);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    pthread_exit(NULL);
}

int randomNumber() {
    srand(time(NULL));
    int random_number = rand();
    return random_number;
}

void init() {
    if (CURRENT_LEVEL == 1) {
        int i = system(str_concat("mkdir ", workspace_main));
        int j = system(str_concat("mkdir ", str_concat(workspace_main, workspance_links)));
    }
    //Apagando anteriores
    if (CURRENT_LEVEL == 1 && OVERIDE_OLD_FILES) {
        //        FILE * a = fopen(FILENAME_LINKS, "w");
        //        if (a != NULL)
        //            fclose(a);
        //        FILE * b = fopen(FILENAME_OTHERFILES, "w");
        //        if (b != NULL)
        //            fclose(b);
        //        FILE * c = fopen(FILENAME_OTHERDOMAINS, "w");
        //        if (a != NULL)
        //            fclose(c);


        int i = system(str_concat("rm ", FILENAME_OTHERDOMAINS));
        i = system(str_concat("rm ", FILENAME_OTHERFILES));
        i = system(str_concat("rm ", FILENAME_LINKS));
    }

}

void ending() {
    if (CURRENT_LEVEL == 1 && ERASE_WORKSPACE_FOLDER) {
        int i = system("rm -rf workspace_crawler");
    }
}

/** argumento 1 será o link
 * argumento 2 será o nível de procura (até 5)
 */
int schemeMAIN(char * url, int nivel) {

    if (checkIfLinkWasDownloaded(url)) {
        char * msgErro = "LINK JÁ FOI BAIXADO";
        //logs(str_concat(str_concat(msgErro, ":\t"), url));
        writeLinkOnFileNotDownloaded(msgErro, url);
        return 0;
    } else if (nivel > LEVEL_ALLOWED) {
        char * msgErro = "PROIBIDO BAIXAR ACIMA DO NIVEL 5";
        //logs(str_concat(str_concat(msgErro, ":\t"), url));
        writeLinkOnFileNotDownloaded(msgErro, url);
        return 0;
    }
    int pid = getpid();
    //writeLinkOnFileDownloaded(url); /*Tirei de depois do wget pra evitar q dois links comecem o download
    /*ao mesmo tempo, por causa demora do download*/

    //logs("-----------------------------------------");
    //logs("schemeMAIN()");
    printf("main(): PARENT_PID: %d\tPID: %d\tCURRENT_LEVEL: %d/%d\tURL: %s\n", getppid(), pid, nivel, LEVEL_ALLOWED, url);

    int qntd = 0;
    int num = randomNumber();
    char nomeArquivo[40] = "", command[150], path[50];

    sprintf(nomeArquivo, "%d%d.txt", num, pid);
    //logs(nomeArquivo);
    sprintf(path, str_concat(str_concat("./", workspace_main), "%s"), nomeArquivo);
    sprintf(command, "wget -q --output-document=%s --timeout=%d --tries=%d --wait=%d ", path, TIMEOUT_TO_DOWNLOAD, TRIES_TO_DOWNLOAD, WAIT_TO_DOWNLOAD);
    //sprintf(command, "wget -q --output-document=%s ", path);
    char * cfinal = str_concat(command, url);
    //logs(str_concat("COMMAND TO RUN: ", cfinal));

    int res = system(cfinal);

    if (res == 0) {
        writeLinkOnFileDownloaded(url);
        char * arq = parserINIT(nomeArquivo, path, url);

        qntd = getLinesFromFile(arq);

        repeatScheme(arq, nivel);

    } else {
        char * erroMsg = "ERROR: INVALID URL, OR SERVER DOESN'T ANSWERING, OR SYSTEM DOESN'T SUPPORT 'wget' CALL: ";

        writeLinkOnFileNotDownloaded(erroMsg, url);
        logs(str_concat(erroMsg, url));
    }

    return qntd;
}

void repeatScheme(char * txt, int nv) {
    //logi(masterPID);
    char * full = readfile(txt);
    if (full == NULL) {
        return;
    }
    char** linhasArr = str_split(full, '\n');
    pid_t child_pid, wpid;
    int status = 0, i;

    //Father code (before child processes start)
    for (i = 0; *(linhasArr + i); i++) {
        if (!checkIfLinkWasDownloaded(*(linhasArr + i)) && nv + 1 <= LEVEL_ALLOWED) {//Se link já foi baixado, nem crio um novo processo
            if ((child_pid = fork()) == 0) {
                //printf("Processo filho(%d): contador=%d\n", getpid(), i);
                //printf("Criou pid: %d\t%s\n", getpid(), *(linhasArr + i));
                //forkCreated(*(linhasArr + i));

                char * args = CURRENT_FILE_NAME;
                args = str_concat(args, str_concat(" -link=", *(linhasArr + i)));
                //char * args = str_concat("-link=", *(linhasArr + i));
                char str1[20];
                sprintf(str1, "%d", LEVEL_ALLOWED);

                args = str_concat(args, str_concat(" -level=", str1));
                if (EXT_PARAM != NULL) {
                    args = str_concat(args, str_concat(" ", EXT_PARAM));
                    //logs(EXT_PARAM);
                }

                if (EXPLICIT) {
                    args = str_concat(args, " --explicit");
                }
                if (!ERASE_WORKSPACE_FOLDER) {
                    args = str_concat(args, " --noerase");
                }
                if (SAVE_EMAIL) {
                    args = str_concat(args, " --email");
                }
                if (SAVE_LINKS_OTHERDOMAINS) {
                    args = str_concat(args, " --otherdomains");
                }
                if (SAVE_LINKS_OTHERFILES) {
                    args = str_concat(args, " --otherfiles");
                }

                char strT1[10];
                sprintf(strT1, "%d", TRIES_TO_DOWNLOAD);
                args = str_concat(args, str_concat(" -wtries=", strT1));
                //                logi(TRIES_TO_DOWNLOAD);
                //                logs(strT1);
                char strT2[10];
                sprintf(strT2, "%d", TIMEOUT_TO_DOWNLOAD);
                args = str_concat(args, str_concat(" -wtimeout=", strT2));
                //                logi(TIMEOUT_TO_DOWNLOAD);
                //                logs(strT2);
                char strT3[10];
                sprintf(strT3, "%d", WAIT_TO_DOWNLOAD);
                args = str_concat(args, str_concat(" -wwait=", strT3));
                //                logi(WAIT_TO_DOWNLOAD);
                //                logs(strT3);

                /*REMOVER*/
                //CURRENT_LEVEL = nv;
                /**/

                char str2[20];
                sprintf(str2, "%d", (CURRENT_LEVEL + 1));
                args = str_concat(args, str_concat(" -nv=", str2));

                //logs(CURRENT_FILE_NAME);
                //logs(args);

                char *envp[] = {
                    "HOME=/",
                    "PATH=/bin:/usr/bin",
                    "TZ=UTC0",
                    0
                };

                execve(CURRENT_FILE_NAME, str_split(args, ' '), envp);
                exit(0);
            }
        } else {
            char * msgErro = "LINK JÁ FOI BAIXADO";
            //logs(str_concat(str_concat(msgErro, ":\t"), url));
            writeLinkOnFileNotDownloaded(msgErro, *(linhasArr + i));
        }
    }

    while ((wpid = wait(&status)) > 0); // this way, the father waits for all the child processes 
}

void forkCreated(char * url) {
    printf("forkCreated() %s\n", url);
    char * workPath1 = str_concat(str_concat("./", workspace_main), FILENAME_PROCESS_COUNTER);

    FILE * arq = fopen(workPath1, "a");
    if (arq != NULL) {
        fprintf(arq, "1\n");
        fclose(arq);
    }
}

void abortingCauseByParameter(char * param) {
    if (param != NULL) {
        printf("PARAMETER %s HAS A BAD FUNCTIONING.\n", param);
        printf("...Aborting...\n\n");
    } else {
        printf("LINK IS REQUIRED TO THE PROCESS. INSERT ONE...\n");
        printf("...Aborting...\n\n");
    }
    exit(-1);
}

int main(int argc, char *argv[]) {
    //printf("main() INICIO pid: %d\n", getpid());
    CURRENT_FILE_NAME = argv[0];

    //    for (int ai = 0; ai < argc; ai++) {
    //        printf("argc[%d] = %s\n", ai, argv[ai]);
    //    }
    //exit(0);

    //1 - COLOCAR EXECL

    char * urlToUseCrawler;
    if (1 == 1) {
        if (argv[1] != NULL && str_equals("--help", argv[1])) {
            printHelp();
        }
        int ai;
        urlToUseCrawler = "";

        int required = 0;
        for (ai = 0; ai < argc; ai++) {
            if (required == 0 && str_startsWith((argv[ai]), "-link=")) {
                required = 1;
                urlToUseCrawler = argv[ai];
                memmove(urlToUseCrawler, urlToUseCrawler + 6, strlen(urlToUseCrawler));
                if (strlen(urlToUseCrawler) < 1)
                    abortingCauseByParameter("LINK");

            } else if (str_startsWith(argv[ai], "-level=")) {
                int tam;
                char * l = argv[ai];
                memmove(l, l + 7, strlen(l));
                if (strlen(l) >= 1 && strlen(l) <= 2) {
                    int vr = atoi(l);
                    if (vr > 10 || vr < 1)
                        abortingCauseByParameter("LEVEL");
                    LEVEL_ALLOWED = vr;
                } else {
                    abortingCauseByParameter("LEVEL");
                }

            } else if (str_startsWith(argv[ai], "-ext=")) {
                char * l = argv[ai];
                EXT_PARAM = str_copy(argv[ai]);
                //strcpy(EXT_PARAM, argv[ai]);
                memmove(l, l + 5, strlen(l));
                //logs(str_concat("EXTENSOES: ", l));
                if (str_contains(l, ",")) {
                    char** arr = str_split(l, ',');

                    char ** newArray;
                    int i, i1;
                    for (i1 = 0; *(arr + i1); i1++) {
                    }
                    newArray = malloc(i1 * sizeof (char*));
                    for (i = 0; i < i1; ++i) {
                        newArray[i] = (char *) malloc(10);
                    }
                    for (i = 0; *(arr + i); i++) {
                        *(newArray + i) = str_concat(".", *(arr + i));
                        //logs(*(newArray + i));
                    }
                    //logi(i1);

                    setExtensionsAllowed(newArray, i1);
                } else {
                    char* arr[] = {str_concat(".", l)}; //str_split(l, ',');
                    setExtensionsAllowed(arr, 1);
                }
            } else if (str_startsWith(argv[ai], "--noerase")) {
                ERASE_WORKSPACE_FOLDER = 0;
            } else if (str_startsWith(argv[ai], "--explicit")) {
                EXPLICIT = 1;
            } else if (str_startsWith(argv[ai], "--email")) {
                SAVE_EMAIL = 1;
            } else if (str_startsWith(argv[ai], "--otherdomains")) {
                SAVE_LINKS_OTHERDOMAINS = 1;
            } else if (str_startsWith(argv[ai], "--otherfiles")) {
                SAVE_LINKS_OTHERFILES = 1;
            } else if (str_startsWith(argv[ai], "-nv=")) {
                char * l = argv[ai];

                memmove(l, l + 4, strlen(l));

                CURRENT_LEVEL = str_stringToInt(l);
                //logi(CURRENT_LEVEL);
            } else if (str_startsWith(argv[ai], "-wtries=")) {
                char * l = argv[ai];

                memmove(l, l + 8, strlen(l));

                TRIES_TO_DOWNLOAD = str_stringToInt(l);
            } else if (str_startsWith(argv[ai], "-wtimeout=")) {
                char * l = argv[ai];

                memmove(l, l + 10, strlen(l));

                TIMEOUT_TO_DOWNLOAD = str_stringToInt(l);
            } else if (str_startsWith(argv[ai], "-wwait=")) {
                char * l = argv[ai];

                memmove(l, l + 7, strlen(l));

                WAIT_TO_DOWNLOAD = str_stringToInt(l);
            }
        }

        if (!required) {
            printHelp();
        }
    }



    //    char ** extensoes = getExtensionsAllowed();
    //    int tam = getExtensionsAllowedSize(), i1;
    //    for (i1 = 0; i1 < getExtensionsAllowedSize(); i1++) {
    //        printf("-->EXTENSOES ALLOWED[%d]: %s\n", i1, extensoes[i1]);
    //    }
    //    extensoes = getExtensionsProhibited();
    //    for (i1 = 0; i1 < getExtensionsProhibitedSize(); i1++) {
    //        printf("-->EXTENSOES PROHIBITED[%d]: %s\n", i1, *(extensoes + i1));
    //    }

    struct timeval inicio, final;
    int tmili;
    gettimeofday(&inicio, NULL);


    //logs(urlToUseCrawler);
    forkCreated(urlToUseCrawler);

    init();
    int qntd_links = 0;

    if (USE_LOCAL_INDEX_HTML == 0) {
        //urlToUseCrawler = "www.jpcontabil.com/crowler";
        //urlToUseCrawler = "www.openbsd.org";

        if (CURRENT_LEVEL == 1) {//ANOTA a primeira URL APENAS. As demais seão escritas a medida q forem sendo encontradas
            writeAndEnumerate(urlToUseCrawler);
        }

        char * workPath = str_concat(str_concat(str_concat("./", workspace_main), workspance_links), "0a.txt");

        FILE * arq = fopen(workPath, "w+");
        if (arq != NULL) {
            fprintf(arq, "%s\n", urlToUseCrawler);
            fclose(arq);
        }

        qntd_links = schemeMAIN(urlToUseCrawler, CURRENT_LEVEL);

        //removeDuplicatedLinksFolder();
        //enumerateAndSave();
    }
    //    } else {
    //        int num = randomNumber();
    //        char nomeArquivo[100];
    //
    //        sprintf(nomeArquivo, "%d%d.txt", num, getpid());
    //
    //        char * URL_DOMAIN = "";
    //char * URL_DOMAIN = "www.openbsd.org";
    //char* URL_DOMAIN = "www.garanhuns.pe.gov.br";
    //char * URL_DOMAIN = "www.superdownloads.com.br";
    //char * URL_DOMAIN = "www.ufrpe.br";
    //char * URL_DOMAIN = "en.wikipedia.org/";

    //qntd_links = parserINIT(nomeArquivo, "./pags/index.html", URL_DOMAIN);
    //removeDuplicatedLinks();
    //        enumerateAndSave();
    //    }


    //printf("main() FIM pid: %d\n", getpid());
    if (CURRENT_LEVEL == 1) {
        printf("\n STATISTICS:\n");

        int linhas = getLinesFromFile(FILENAME_LINKS);
        printf(" %d valid link(s) found\n", linhas);

        linhas = getLinesFromFile(FILENAME_EMAIL);
        if (linhas != -1) {
            printf(" %d email(s) found\n", linhas);
        }

        linhas = getLinesFromFile(FILENAME_OTHERFILES);
        if (linhas != -1) {
            printf(" %d file(s) with prohibited extension found\n", linhas);
        }

        linhas = getLinesFromFile(FILENAME_OTHERDOMAINS);
        if (linhas != -1) {
            printf(" %d link(s) from others domains found\n", linhas);
        }

        linhas = getLinesFromFile(str_concat(str_concat("./", workspace_main), FILENAME_LINKS_DOWNLOADED));
        if (linhas != -1) {
            printf(" %d page(s) mapped\n", linhas);
        } /*else {
            printf(" 0 page(s) mapped\n");
        }*/

        linhas = getLinesFromFile(str_concat(str_concat("./", workspace_main), FILENAME_LINKS_NOT_DOWNLOADED));
        if (linhas != -1) {
            printf(" %d error(s) while trying to download page(s)\n", (linhas / 2)); //tiro o \n do final? testar isso...
        }

        linhas = getLinesFromFile(str_concat(str_concat("./", workspace_main), FILENAME_PROCESS_COUNTER));
        if (linhas != -1) {
            printf(" %d process(es) created\n", linhas + 1); //tiro o \n do final? testar isso...
        }

        ending();


        /*MEASURE*/
        gettimeofday(&final, NULL);
        float tmiliF = (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
        tmili = (int) tmiliF;


        int seconds = (int) (tmiliF / 1000) % 60;
        int minutes = (int) ((int) (tmiliF / 60000) % 60);
        int hours = (int) ((int) (tmiliF / 3600000) % 24);
        int milliseconds = (int) tmiliF - (int) (hours * 3600000)-(int) (minutes * 60000) - (int) (seconds * 1000);
        printf("\tELAPSED TIME: %dh %dmin %ds %dms\n", hours, minutes, seconds, milliseconds);
    }

    return (EXIT_SUCCESS);
}

void printHelp() {
    printf("\t    #############################################\n");
    printf("\t    #############################################\n");
    printf("\t    #############         ########  2017  #######\n");
    printf("\t    ############# CRAWLER ##### by: PMateus #####\n");
    printf("\t    #############         ########   1.0  #######\n");
    printf("\t    #############################################\n");
    printf("\t    #############################################\n\n\n");
    printf("Parameters (* = OPTIONAL):\n\n");
    printf("\tlink             | choose one link to make crawler work.\n\n");
    printf("\tlevel*           | set the deep level of crawler (default: 5).\n");
    printf("\t                   You can set this from 1 to 10.\n\n");
    printf("\text*             | set the allowed files extensions that \n");
    printf("\t                   crawler can access (default: html and htm).\n");
    printf("\t                   Use common (,) to separate extensions.\n\n");
    printf("\tnoerase*         | if you set this, all files (pages) catched\n");
    printf("\t                   will be available to you inside a folder\n");
    printf("\t                   named 'workspace_crawler'.\n\n");
    printf("\texplicit*        | if you set this, the crawler only will map \n");
    printf("\t                   pages with explicit extensions. Example: \n");
    printf("\t                   'www.openbsd.org/panel' won't be mapped.\n");
    printf("\t                   'www.openbsd.org/panel/index.html' will be mapped.\n\n");
    printf("\temail*           | if you set this, the program will save every \n");
    printf("\t                   email found on file named 'links_emails.txt'.\n\n");
    printf("\totherdomains*    | if you set this, the program will save every\n");
    printf("\t                   link of other domain on a file named\n");
    printf("\t                   'links_otherDomains.txt'.\n\n");
    printf("\totherfiles*      | if you set this, the program will save every\n");
    printf("\t                   file with prohibited extension on a file named \n");
    printf("\t                   'links_otherDomains.txt'.\n\n");
    printf("\twtimeout*        | set the timeout (in seconds) for program wait for\n");
    printf("\t                   the server response. [DEFAULT 20s].\n\n");
    printf("\twtries*          | set the program attemps to download page if \n");
    printf("\t                   couldn't yet. [DEFAULT 3].\n\n");
    printf("\twwait*           | set this for program wait (in seconds) between\n");
    printf("\t                   each re-download attemp. [DEFAULT 10s].\n\n");
    printf("HOW TO:\n");
    printf("\t./crawler -link=http://...com -level=3\n");
    printf("\t./crawler -link=www...org -ext=html,htm\n");
    printf("\t./crawler -link=www...com/painel --explicit --email --otherdomains --otherfiles\n");
    printf("\t./crawler -link=subdomain.maindomain.com --noerase -level=5 -ext=html,htm,php --explicit -wwait=10 -wtimeout=10 -wtries=5\n\n\n");
    printf("\t#######################################################\n");
    printf("\t                      OBSERVATIONS\n");
    printf("\t#######################################################\n\n");
    printf(" 1- We strong recommend you just use this software only on websites you own. Otherwise you can cause unnecessary problems...\n");
    printf(" 2- The crawler doesn't work for different domains. If the website www.test.com has a google link like www.google.com/accessIt, the crawler will dump this link as 'otherDomain', and will block further actions.\n");
    printf(" 3- Subdomains won't be mapped. Ex: www.google.com AND groups.google.com.\n");
    printf(" 4- Do NOT include spaces between PARAMETERS and their CONTENT, or it won't work properly.\n");
    printf(" 5- The parameters order does not matters.\n");
    printf(" 6- Press CTRL+C to quit the process.\n");
    printf(" 7- The entire process may take long time. We advise you to drink some coffee and play a bit at your smartphone.\n\n");

    exit(0);
}
