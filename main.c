/******************************************************************************
 * FILE: main.c
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 * LAST REVISED: 04/fev/17
 ******************************************************************************/
/*
 * TODO:
 * 1- Consertar bug que impede que o URL no INPUT possua / no  final
 * 2- Remover links duplicados
 */


#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex.h>  
//#include <assert.h>
#include <ctype.h>

#include "parser.h"
#include "settings.h"
#include "methods.h"
#include "stringmethods.h"
//#include "methods.h"
//#include "stringmethods.h"

#define NUM_THREADS 5
//#define USE_LOCAL_INDEX_HTML 1

//char * workspace_main_folder = "workspace_crowler";
//char * workspace_links_folder = "/links";

void *PrintHello(void *threadid) {
    long tid;
    tid = (long) threadid;
    printf("Do stuff. Thread id: #%ld, NUMBER: %d!\n", tid, randomNumber());
    //printf("pID: %d\n", getpid());

    pthread_exit(NULL);
}

int createThread(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    for (t = 0; t < NUM_THREADS; t++) {
        printf("main() creating thread: #%ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *) t);
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
    // int i = system(str_concat("mkdir ", workspace_main_folder));

    //int j = system(str_concat(str_concat("mkdir -p ", workspace_main_folder), workspace_links_folder));

    int i = system(str_concat("mkdir ", workspace_main));
    int j = system(str_concat("mkdir ", str_concat(workspace_main, workspance_links)));

}

void ending() {
    int i = system("rm -rf workspace_crowler");

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

    if (res == 0) {
        logs("URL DOWNLOADED");
        logs(str_concat("DOMAIN: ", url));
        logs(str_concat("PATH: ", path));
        logs(str_concat("FILENAME: ", nomeArquivo));

        qntd = parserINIT(nomeArquivo, path, url);

    } else {
        logs(str_concat("ERROR: INVALID URL OR SERVER DOESN'T WORKING: ", url));
    }

    return qntd;
}

int main(int argc, char *argv[]) {
    logs("main()");
    init();
    int qntd_links = 0;

    if (USE_LOCAL_INDEX_HTML == 0) {

        //qntd_links = schemeMAIN("www.openbsd.org", 0, 0);
        qntd_links = schemeMAIN("www.baixaki.com.br", 0, 0);
        //qntd_links = schemeMAIN("www.superdownloads.com.br", 0, 0);
        //        qntd_links = schemeMAIN("www.garanhuns.pe.gov.br", 0, 0);

    } else {
        int num = randomNumber();
        char nomeArquivo[100], path[100];

        sprintf(nomeArquivo, "%d%d.txt", num, getpid());
        sprintf(path, str_concat(str_concat("./", workspace_main), "%s"), nomeArquivo);

        qntd_links = parserINIT("indexPMG.txt", "./pags/indexPMG.html", URL_DOMAIN); //LEMBRAR DE ALTERAR URL_DOMAIN
    }

    ending();


    printf("QUANTIDADE DE LINKS: %d", qntd_links);

    return (EXIT_SUCCESS);
}