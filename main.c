/******************************************************************************
 * FILE: main.c
 * DESCRIPTION:
 *   Crowler to map whole website (webpages, imagens, style files, etc).
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

int masterPID = -1;

void printHelp();

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
    int i = system(str_concat("mkdir ", workspace_main));
    int j = system(str_concat("mkdir ", str_concat(workspace_main, workspance_links)));

    //Apagando anteriores
    if (OVERIDE_OLD_FILES) {

        FILE * a = fopen(FILENAME_LINKS, "w");
        if (a != NULL)
            fclose(a);
        FILE * b = fopen(FILENAME_OTHERFILES, "w");
        if (b != NULL)
            fclose(b);
        FILE * c = fopen(FILENAME_OTHERDOMAINS, "w");
        if (a != NULL)
            fclose(c);
    }

}

void ending() {
    if (ERASE_WORKSPACE_FOLDER) {
        int i = system("rm -rf workspace_crowler");
    }
}

/** argumento 1 será o link
 * argumento 2 será o nível de procura (até 5)
 */
int schemeMAIN(char * url, int nivel, long threadId) {

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
    //writeLinkOnFileDownloaded(url); /*Tirei de depois do wget pra evitar q dois links comecem o download
    /*ao mesmo tempo, por causa demora do download*/

    //logs("-----------------------------------------");
    //logs("schemeMAIN()");
    printf("LOG: schemeMAIN(): PID: %d\tTHREAD_ID: %ld \tLEVEL: %d \tURL: %s\n", getpid(), threadId, nivel, url);

    int qntd = 0;
    int num = randomNumber();
    char nomeArquivo[100], command[100], path[100];

    sprintf(nomeArquivo, "%d%d.txt", num, getpid());
    sprintf(path, str_concat(str_concat("./", workspace_main), "%s"), nomeArquivo);
    sprintf(command, "wget -q --output-document=%s ", path);
    char * cfinal = str_concat(command, url);
    //logs(str_concat("COMMAND TO RUN: ", cfinal));

    int res = system(cfinal); //-q não mostrar output
    //logi(res);
    if (res == 0) {
        writeLinkOnFileDownloaded(url);

        //logs("URL DOWNLOADED");
        //logs(str_concat("DOMAIN: ", url));
        //logs(str_concat("PATH: ", path));
        //logs(str_concat("FILENAME: ", nomeArquivo));

        char * arq = parserINIT(nomeArquivo, path, url);

        //char * a = readfile(arq);
        //printf("%s", a);
        qntd = getLinesFromFile(arq);

        repeatScheme(arq, nivel);

    } else if (res == 32512) {
        char * erroMsg = "ERROR: SYSTEM DOESN'T SUPPORT 'wget' CALL: ";
        writeLinkOnFileDownloaded(url);
        writeLinkOnFileNotDownloaded(erroMsg, url);
        logs(str_concat(erroMsg, url));
    } else {
        char * erroMsg = "ERROR: INVALID URL, OR SERVER DOESN'T ANSWERING, OR SYSTEM DOESN'T SUPPORT 'wget' CALL: ";
        writeLinkOnFileDownloaded(url);
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
        if (!checkIfLinkWasDownloaded(*(linhasArr + i))) {//Se link já foi baixado, nem crio um novo processo
            if ((child_pid = fork()) == 0) {
                //printf("Processo filho(%d): contador=%d\n", getpid(), i);
                //printf("Criou pid: %d\t%s\n", getpid(), *(linhasArr + i));
                forkCreated();




                schemeMAIN(*(linhasArr + i), nv + 1, 0);
                //printf("Vai finalizar processo: %d\t%s\n", getpid(), *(linhasArr + i));
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

void forkCreated() {
    char * workPath1 = str_concat(str_concat("./", workspace_main), "processesCount.txt");

    FILE * arq = fopen(workPath1, "a+");
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
    //1 - COLOCAR EXECL
    //1.2 - PASSAR NIVEL ATUAL POR UM PARAMETRO NO INPUT (-nv=NUM)
    //2 - COLOCAR PRA A MEDIDA QUE ACHAR NOVOS LINKS, ADICIONÁ-LOS JÁ NO ARQUIVO FINAL (VERIFICAR SE JÁ EXISTE) E NUMERÁ-LOS

    //check for help
    char * urlToUseCrowler;
    if (1 == 1) {
        if (argv[1] != NULL && str_equals("--help", str_toLowerCase(argv[1]))) {
            printHelp();
            exit(0);
        }
        int ai;
        urlToUseCrowler = "";

        int required = 0;
        for (ai = 0; ai < argc; ai++) {
            if (required == 0 && str_startsWith((argv[ai]), "-link=")) {
                required = 1;
                urlToUseCrowler = argv[ai];
                memmove(urlToUseCrowler, urlToUseCrowler + 6, strlen(urlToUseCrowler));
                if (strlen(urlToUseCrowler) < 1)
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

                //logs("NO FIM DO IF2");
            } else if (str_startsWith(argv[ai], "--noerase")) {
                ERASE_WORKSPACE_FOLDER = 0;
            } else if (str_startsWith(argv[ai], "--explicit")) {
                EXPLICIT = 1;
            }
        }


        //        char ** extensoes = getExtensionsAllowed();
        //        int tam = getExtensionsAllowedSize(), i1;
        //        for (i1 = 0; i1 < getExtensionsAllowedSize(); i1++) {
        //            printf("-->EXTENSOES ALLOWED[%d]: %s\n", i1, extensoes[i1]);
        //        }
        //        extensoes = getExtensionsProhibited();
        //        for (i1 = 0; i1 < getExtensionsProhibitedSize(); i1++) {
        //            printf("-->EXTENSOES PROHIBITED[%d]: %s\n", i1, *(extensoes + i1));
        //        }

        if (!required) {
            printHelp();
            exit(0);
        }
    }
    //logs(argv[1]);
    //execl("./crowler1", NULL);
    //execl("cd /dist/Debug/GNU-Linux/ && ./crowler1", NULL);
    //exit(0);

    struct timeval inicio, final;
    int tmili;
    gettimeofday(&inicio, NULL);


    forkCreated();

    //logs("main()");
    init();
    int qntd_links = 0;

    if (USE_LOCAL_INDEX_HTML == 0) {
        //char * urlToUseCrowler = "www.jpcontabil.com/wp/";
        //urlToUseCrowler = "www.jpcontabil.com/crowler/index.html";
        //urlToUseCrowler = "www.jpcontabil.com/crowler/";
        //urlToUseCrowler = "www.jpcontabil.com/crowler";
        //char * urlToUseCrowler = "www.openbsd.org";

        char * workPath = str_concat(str_concat(str_concat("./", workspace_main), workspance_links), "0a.txt");
        FILE * arq = fopen(workPath, "w+");
        if (arq != NULL) {
            fprintf(arq, "%s\n", urlToUseCrowler);
            fclose(arq);
        }

        qntd_links = schemeMAIN(urlToUseCrowler, 1, 0);

        removeDuplicatedLinksFolder();
        enumerateAndSave();
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

    //Garantir que método seja chamado apenas pela última thread/processo

    //logs("acabou");
    ending();

    /*MEASURE*/
    gettimeofday(&final, NULL);
    float tmiliF = (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
    tmili = (int) tmiliF;

    int seconds = (int) (tmiliF / 1000) % 60;
    int minutes = (int) ((int) (tmiliF / 60000) % 60);
    int hours = (int) ((int) (tmiliF / 3600000) % 24);

    //printf("TEMPO DECORRIDO: %dh %dmin %ds      ||      %.3fs (%dms)\n", hours, minutes, seconds, (tmiliF / 1000), (int) tmiliF);

    return (EXIT_SUCCESS);
}

void printHelp() {
    printf("\t    #############################################\n");
    printf("\t    #############################################\n");
    printf("\t    #############         ########  2017  #######\n");
    printf("\t    ############# CROWLER ##### by: PMateus #####\n");
    printf("\t    #############         ########   1.0  #######\n");
    printf("\t    #############################################\n");
    printf("\t    #############################################\n\n\n");
    printf("Parameters (* = OPTIONAL):\n\n");
    printf("\tlink             | choose one link to make crowler work.\n\n");
    printf("\tlevel*           | set the deep level of crowler (default: 5).\n");
    printf("\t                   You can set this from 1 to 10.\n\n");
    printf("\text*             | set the allowed files extensions that \n");
    printf("\t                   crowler can access (default: html and htm).\n");
    printf("\t                   Use common (,) to separate extensions.\n\n");
    printf("\tnoerase*         | if you use this, all files (pages) catched\n");
    printf("\t                   will be available to you inside a folder\n");
    printf("\t                   named 'workspace_crowler'.\n\n");
    printf("\texplicit*        | if you use this, the crowler only will map \n");
    printf("\t                   pages with explicit extensions. Example: \n");
    printf("\t                   'www.openbsd.org/panel' won't be mapped.\n");
    printf("\t                   'www.openbsd.org/panel/index.html' will be mapped.\n\n");
    printf("HOW TO:\n");
    printf("\t./crowler -link=http://...com -level=3\n");
    printf("\t./crowler -link=www...org -ext=html,htm\n");
    printf("\t./crowler -link=www...com/painel --explicit\n");
    printf("\t./crowler -link=subdomain.maindomain.com --noerase -level=5 -ext=html,htm --explicit\n\n\n");
    printf("\t#######################################################\n");
    printf("\t                      OBSERVATIONS\n");
    printf("\t#######################################################\n\n");
    printf(" 1- We strong recommend you just use this software only on websites you own. Otherwise you can cause unnecessary problems...\n");
    printf(" 2- The crowler doesn't work for different domains, that is, if the website www.test.com has a google link like www.google.com/accessIt, the crowler will dump this link as 'otherDomain', and will block further actions.\n");
    printf(" 3- Subdomains won't be mapped. Ex: www.google.com AND groups.google.com.\n");
    printf(" 4- Do NOT include spaces between PARAMETERS and their CONTENT, or it won't work properly.\n");
    printf(" 5- The parameters order does not matters.\n");
    printf(" 6- Press CTRL+C to quit the process.\n");
    printf(" 7- The entire process may take long time. We advise you to drink some coffee and play a bit at your smartphone.\n\n");
}
