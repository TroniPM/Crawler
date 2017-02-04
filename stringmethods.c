/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <string.h>
//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex.h>  
//#include <assert.h>
#include <ctype.h>

#include "methods.h"

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

char** str_split(char* a_str, const char a_delim) {
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

/*
 return 1 if has link
 return 0 if hasn't link
 */
int checkIfLineContainsLink(char * line) {
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

int str_endsWith(const char *str, const char *suffix) {
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}