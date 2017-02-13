/******************************************************************************
 * FILE: stringmethods.c
 * DESCRIPTION:
 *   Crowler to map whole website (webpages, imagens, style files, etc).
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex.h>
#include <ctype.h>

#include "methods.h"
#include "stringmethods.h"
#include "settings.h"

int getLinesFromFile(char * txt) {
    int lines = 0;
    FILE * arq = fopen(txt, "r");
    if (arq != NULL) {
        lines = 1;
        int ch = 0;

        while (!feof(arq)) {
            ch = fgetc(arq);
            if (ch == '\n' || ch == '\r') {
                lines++;
            }
        }
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

char* str_trim(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char) *str)) str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) end--;

    // Write new null terminator
    *(end + 1) = 0;

    return str;
}

char* str_concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); //+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); //+1 to copy the null-terminator
    return result;
}

int str_countOcurrences(char *search, char *subject) {
    char *p = NULL;
    int c = 0, search_size;

    search_size = strlen(search);

    //Count how many occurences
    for (p = strstr(subject, search); p != NULL; p = strstr(p + search_size, search)) {
        c++;
    }

    return c;
}

/**
 * char *a = "allblldddlll";
 * char *c = str_replace("ll" , "xll" , a);
 * @param search: substring a ser procurada
 * @param replace: substring q irá substituir
 * @param subject: string original
 * @return nova string
 */
char* str_replace(char *search, char *replace, char *subject) {
    char *p = NULL, *old = NULL, *new_subject = NULL;
    int c = 0, search_size;

    search_size = strlen(search);

    //Count how many occurences
    for (p = strstr(subject, search); p != NULL; p = strstr(p + search_size, search)) {
        c++;
    }

    //Final size
    c = (strlen(replace) - search_size) * c + strlen(subject);

    //New subject with new size
    new_subject = malloc(c);

    //Set it to blank
    strcpy(new_subject, "");

    //The start position
    old = subject;

    for (p = strstr(subject, search); p != NULL; p = strstr(p + search_size, search)) {
        //move ahead and copy some text from original subject , from a certain position
        strncpy(new_subject + strlen(new_subject), old, p - old);

        //move ahead and copy the replacement text
        strcpy(new_subject + strlen(new_subject), replace);

        //The new start position after this search match
        old = p + search_size;
    }

    //Copy the part after the last search match
    strcpy(new_subject + strlen(new_subject), old);

    return new_subject;
}

/**
    char **tokens;
    size_t numtokens;
    tokens = split(line, ", \t\n", &numtokens);
    for (size_t i = 0; i < numtokens; i++) {
        printf("    token: \"%s\"\n", tokens[i]);
        free(tokens[i]);
    }
 * @param str
 * @param delim
 * @param numtokens
 * @return 
 */
char **split(const char* str, const char* delim, size_t* numtokens) {
    // copy the original string so that we don't overwrite parts of it
    // (don't do this if you don't need to keep the old line,
    // as this is less efficient)
    char *s = strdup(str);
    // these three variables are part of a very common idiom to
    // implement a dynamically-growing array
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof (char*));
    char *token, *strtok_ctx;
    for (token = strtok_r(s, delim, &strtok_ctx);
            token != NULL;
            token = strtok_r(NULL, delim, &strtok_ctx)) {
        // check if we need to allocate more space for tokens
        if (tokens_used == tokens_alloc) {
            tokens_alloc *= 2;
            tokens = realloc(tokens, tokens_alloc * sizeof (char*));
        }
        tokens[tokens_used++] = strdup(token);
    }
    // cleanup
    if (tokens_used == 0) {
        free(tokens);
        tokens = NULL;
    } else {
        tokens = realloc(tokens, tokens_used * sizeof (char*));
    }
    *numtokens = tokens_used;
    free(s);
    return tokens;
}

/**
    char** arr = str_split(stringToSplit, '\n');
    int i;
    for (i = 0; *(arr + i); i++) {
    }
 * @param a_str
 * @param a_delim
 * @return 
 */
char** str_split(char* a_str, const char a_delim) {
    //logs(str_concat("str_split() ", a_str));
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof (char*) * count);

    if (result) {
        size_t idx = 0;
        char* token = strtok(a_str, delim);

        while (token) {
            //assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        //assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int str_contains(char * original, char * tofind) {
    if (strstr(original, tofind) != NULL) {
        return 1;
    } else {
        return 0;
    }
}

/*
== PASS ==  0: substr(buffer, 20, "string", 0) = ""
== PASS ==  1: substr(buffer, 20, "string", 1) = "s"
== PASS ==  2: substr(buffer, 20, "string", 2) = "st"
== PASS ==  3: substr(buffer, 20, "string", 3) = "str"
== PASS ==  4: substr(buffer, 20, "string", 4) = "stri"
== PASS ==  5: substr(buffer, 20, "string", 5) = "strin"
== PASS ==  6: substr(buffer, 20, "string", 6) = "string"
== PASS ==  7: substr(buffer, 20, "string", 7) = "string"
== PASS ==  8: substr(buffer, 20, "string", -1) = "g"
== PASS ==  9: substr(buffer, 20, "string", -2) = "ng"
== PASS == 10: substr(buffer, 20, "string", -3) = "ing"
== PASS == 11: substr(buffer, 20, "string", -4) = "ring"
== PASS == 12: substr(buffer, 20, "string", -5) = "tring"
== PASS == 13: substr(buffer, 20, "string", -6) = "string"
== PASS == 14: substr(buffer, 20, "string", -7) = "string"
 */
void str_substr(char *buffer, size_t buflen, char const *source, int len) {
    size_t srclen = strlen(source);
    size_t nbytes = 0;
    size_t offset = 0;
    size_t sublen;

    if (buflen == 0) /* Can't write anything anywhere */
        return;
    if (len > 0) {
        sublen = len;
        nbytes = (sublen > srclen) ? srclen : sublen;
        offset = 0;
    } else if (len < 0) {
        sublen = -len;
        nbytes = (sublen > srclen) ? srclen : sublen;
        offset = srclen - nbytes;
    }
    if (nbytes >= buflen)
        nbytes = 0;
    if (nbytes > 0)
        memmove(buffer, source + offset, nbytes);
    buffer[nbytes] = '\0';
}

int str_countOccurrences(char * linha, char * string2find) {
    int count = 0;
    const char *tmp = linha;
    while (tmp = strstr(tmp, string2find)) {
        count++;
        tmp++;
    }

    return count;
}

int str_equals(char * a1, char * a2) {
    if (strcmp(a1, a2) == 0) {
        return 1;
    }
    return 0;
}

int str_endsWith(const char *str, const char *suffix) {
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

int str_startsWith(const char *str, const char *prefix) {
    size_t lenpre = strlen(prefix),
            lenstr = strlen(str);
    return lenstr < lenpre ? 0 : strncmp(prefix, str, lenpre) == 0;
}

char * str_removeLastCharFromString(char* str) {
    //logs("str_removeLastCharFromString");
    char * aux = str;
    //memmove(aux, aux, strlen(aux) - 2);
    int i = strlen(aux);
    aux[i - 1] = 0;
    //logs(aux);
    return aux;
}

char * str_removeFirstCharFromString(char* str) {
    char * aux = str;
    memmove(aux, aux + 1, strlen(aux));

    return aux;
}

char * addBarraAString(char * str) {
    char * aux = str;
    if (!str_endsWith(aux, "/")) {
        aux = str_concat(aux, "/");
    }
    return aux;
}

char * str_toLowerCase(char * str) {
    char * aux = str;
    for (int i = 0; str[i]; i++) {
        aux[i] = tolower(str[i]);
    }

    return aux;
}

char * str_toUpperCase(char * str) {
    char * aux = str;
    for (int i = 0; str[i]; i++) {
        aux[i] = toupper(str[i]);
    }

    return aux;
}

