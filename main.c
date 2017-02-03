/******************************************************************************
 * FILE: main.c
 * DESCRIPTION:
 *   Criado para disciplina de Sistemas Operacionais. Crowler para obter todos
 *   os links possíveis (html e htm) de mesmo domínio de um website.
 * AUTHOR: Paulo Mateus
 * LAST REVISED: 03/fev/17
 ******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


#include <sys/types.h>
#include <unistd.h>

#define NUM_THREADS 5
#define ISDEBUG 1



FILE* currentURL;

void *PrintHello(void *threadid) {
    long tid;
    tid = (long) threadid;
    printf("Do stuff. Thread id: #%ld, NUMBER: %d!\n", tid, randomNumber());
    //printf("pID: %d\n", getpid());

    pthread_exit(NULL);
}

/*int main(int argc, char *argv[]) {

    //struct timeval stop, start;

    //gettimeofday(&start, NULL);

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

    //gettimeofday(&stop, NULL);

    //printf("\nOperation took %lu milliseconds\n", stop.tv_usec - start.tv_usec);
    pthread_exit(NULL);
}*/

int gettime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    unsigned long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
    return time_in_micros;
}

char* concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); //+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); //+1 to copy the null-terminator
    return result;
}

void logs(const char*txt) {
    if (ISDEBUG == 1) {
        printf("\nLOG_S: %s\n", txt);
    }
}

void logi(const int txt) {
    if (ISDEBUG == 1) {
        printf("\nLOG_I: %d\n", txt);
    }
}

int randomNumber() {
    srand(time(NULL));
    int random_number = rand();
    return random_number;
}

void init() {
    logs("init()");
    system("mkdir workspace_crowler");
}

char intToString(int value) {
    char buffer[20];
    snprintf(buffer, 20, "%d", value);
    return buffer;
}

void ending() {
    logs("ending()");
    system("rm -rf workspace_crowler");

    if (currentURL != NULL) {
        fclose(currentURL);
    }
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

void openLinkFile() {
    currentURL = fopen("links.txt", "a+");
}

void writeLinkOnFile(char *txt) {
    if (currentURL != NULL) {
        int ch = 0;
        int lines = 1;
        while (!feof(currentURL)) {
            ch = fgetc(currentURL);
            if (ch == '\n') {
                lines++;
            }
        }

        logi(lines);
        fprintf(currentURL, "%d - %s\n", lines, txt);
    }
}

/** argumento 1 será o link
 * argumento 2 será o nível de procura (até 5)
 * 
 */
int main(int argc, char *argv[]) {
    init();

    char url[] = "www.openbsd.org";
    int num = randomNumber();
    //logi(num);
    char nomeArquivo[100];
    sprintf(nomeArquivo, "./workspace_crowler/%d%d.txt", num, getpid());
    //logs(nomeArquivo);


    char command[50];
    sprintf(command, "wget -q --output-document=%s ", nomeArquivo);

    logs("main() goes to run wget");
    int res = system(concat(command, url)); //-q não mostrar output

    if (res == 0) {
        logs("URL ENCONTRADA");


        //extracting data
        parser();

        openLinkFile();
        if (currentURL != NULL) {
            const char *text = "Write this to the file";
            writeLinkOnFile(text);
        } else {
            logs("Arquivo de LINKS é NULL");
        }
    } else {
        logs(concat("ERRO: URL INVÁLIDO OU SERVIDOR NÃO RESPONDEU DE MANEIRA INESPERADA: ", url));
    }

    ending();
    return (EXIT_SUCCESS);
}

void parser() {
    /**
     * ler arquivo criado (aleatorio) por linha
     * A cada linha, verificar se tal linha possui link (regex)
     * obter conteudo dentro do regex
     * salvar url em um array
     * verificar duplicação de links
     * salvar em arquivo
     */
}
