/******************************************************************************
 * FILE: main.c
 * DESCRIPTION:
 *   Criado para disciplina de Sistemas Operacionais. Crowler para obter todos
 *   os links possíveis (html e htm) de mesmo domínio de um website.
 * AUTHOR: Paulo Mateus
 * LAST REVISED: 03/fev/17
 ******************************************************************************/
//REGEX FOR LINK <a\s+(?:[^>]*?\s+)?href="([^"]*)"
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
//#include "stringmethods.h"
#include "methods.h"

#define NUM_THREADS 5
#define USE_LOCAL_INDEX_HTML 1

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
    int i = system("mkdir workspace_crowler");
}

void ending() {
    system("rm -rf workspace_crowler");

    /*if (currentURL != NULL) {
        fclose(currentURL);
    }*/
}

char* readfile(char *filename) {
    char *buffer = NULL;
    int string_size, read_size;
    FILE *handler = fopen(filename, "r");

    if (handler) {
        // Seek the last byte of the file
        fseek(handler, 0, SEEK_END);
        // Offset from the first to the last byte, or in other words, filesize
        string_size = ftell(handler);
        // go back to the start of the file
        rewind(handler);

        // Allocate a string that can hold it all
        buffer = (char*) malloc(sizeof (char) * (string_size + 1));

        // Read it all in one operation
        read_size = fread(buffer, sizeof (char), string_size, handler);

        // fread doesn't set it so put a \0 in the last position
        // and buffer is now officially a string
        buffer[string_size] = '\0';

        if (string_size != read_size) {
            // Something went wrong, throw away the memory and set
            // the buffer to NULL
            free(buffer);
            buffer = NULL;
        }

        // Always remember to close the file.
        fclose(handler);
    }

    return buffer;
}

/** argumento 1 será o link
 * argumento 2 será o nível de procura (até 5)
 */
int main(int argc, char *argv[]) {
    logs("main()");
    init();
    int qntd_links = 0;

    if (USE_LOCAL_INDEX_HTML == 0) {
        //char url[] = "www.openbsd.org";
        char url[] = "www.garanhuns.pe.gov.br/";
        int num = randomNumber();
        //logi(num);
        char nomeArquivo[100];
        sprintf(nomeArquivo, "./workspace_crowler/%d%d.txt", num, getpid());
        //logs(nomeArquivo);


        char command[100];
        sprintf(command, "wget -q --output-document=%s ", nomeArquivo);

        logs("main() goes to run wget");

        int res = system(str_concat(command, url)); //-q não mostrar output

        if (res == 0) {
            logs("URL DOWNLOADED");
            char* aux = str_concat("FILENAME AND PATH: ", nomeArquivo);
            logs(aux);

            qntd_links = parserINIT(nomeArquivo);

        } else {
            logs(str_concat("ERRO: URL INVÁLIDO OU SERVIDOR NÃO RESPONDEU DE MANEIRA INESPERADA: ", url));
        }
    } else {
        qntd_links = parserINIT("index1.html");
    }

    ending();


    printf("QUANTIDADE DE LINKS: %d", qntd_links);

    return (EXIT_SUCCESS);
}