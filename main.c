/******************************************************************************
 * FILE: main.c
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/
/*
 * TODO:
 * 1- Consertar bug que impede que o URL no INPUT possua / no  final
 */


#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex.h>
#include <ctype.h>

#include "parser.h"
#include "settings.h"
#include "methods.h"
#include "stringmethods.h"

#define NUM_THREADS 15

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

    /*if (currentURL != NULL) {
        fclose(currentURL);
    }*/
}

/** argumento 1 será o link
 * argumento 2 será o nível de procura (até 5)
 */
int schemeMAIN(char * url, int nivel, long threadId) {

    logs("-----------------------------------------");
    logs("schemeMAIN()");
    printf("LOG: schemeMAIN(): THREAD_ID: %ld \tLEVEL: %d \tURL: %s\n", threadId, nivel, url);
    //URL_DOMAIN = url;
    //logs(URL_DOMAIN);

    if (nivel > 5) {
        return 0;
    }

    int qntd = 0;
    int num = randomNumber();
    char nomeArquivo[100], command[100], path[100];

    sprintf(nomeArquivo, "%d%d.txt", num, getpid());
    sprintf(path, str_concat(str_concat("./", workspace_main), "%s"), nomeArquivo);
    sprintf(command, "wget -q --output-document=%s ", path);
    char * cfinal = str_concat(command, url);
    logs(str_concat("COMMAND TO RUN: ", cfinal));

    int res = system(cfinal); //-q não mostrar output
    //logi(res);
    if (res == 0) {
        logs("URL DOWNLOADED");
        logs(str_concat("DOMAIN: ", url));
        logs(str_concat("PATH: ", path));
        logs(str_concat("FILENAME: ", nomeArquivo));

        qntd = parserINIT(nomeArquivo, path, url);

    } else if (res == 32512) {
        logs(str_concat("ERROR: SYSTEM DOESN'T SUPPORT 'wget' CALL: ", url));
    } else {
        logs(str_concat("ERROR: INVALID URL, OR SERVER DOESN'T ANSWERING, OR SYSTEM DOESN'T SUPPORT 'wget' CALL: ", url));
    }

    return qntd;
}

int main(int argc, char *argv[]) {
    //createThread();
    logs("main()");
    init();
    int qntd_links = 0;

    if (USE_LOCAL_INDEX_HTML == 0) {

        //qntd_links = schemeMAIN("www.openbsd.org", 0, 0);
        qntd_links = schemeMAIN("www.openbsd.org", 0, 0);
        //qntd_links = schemeMAIN("www.superdownloads.com.br", 0, 0);
        //qntd_links = schemeMAIN("www.garanhuns.pe.gov.br", 0, 0);

    } else {
        int num = randomNumber();
        char nomeArquivo[100];

        sprintf(nomeArquivo, "%d%d.txt", num, getpid());

        char * URL_DOMAIN = "www.baixaki.com.br";
        //char * URL_DOMAIN = "www.openbsd.org";
        //char* URL_DOMAIN = "www.garanhuns.pe.gov.br";
        //char * URL_DOMAIN = "www.superdownloads.com.br";
        //char * URL_DOMAIN = "www.ufrpe.br";
        //char * URL_DOMAIN = "en.wikipedia.org/";

        qntd_links = parserINIT(nomeArquivo, "./pags/indexBA.html", URL_DOMAIN);
    }

    //Garantir que método seja chamado apenas pela última thread/processo
    removeDuplicatedLinks();
    /*READ file and re do scheme for new valid links*/
    enumerateAndSave();

    ending();
    return (EXIT_SUCCESS);
}