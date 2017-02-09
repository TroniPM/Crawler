/******************************************************************************
 * FILE: stringmethods.h
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/

#ifndef STRINGMETHODS_H
#define STRINGMETHODS_H

int getLinesFromFile(char * txt);
int str_contains(char * original, char * tofind);
int str_countOccurrences(char * linha, char * string2find);
int str_endsWith(const char *str, const char *suffix);
int str_startsWith(const char *str, const char *prefix);
int str_equals(char * a1, char * a2);
void stringmethods_init(char * d);
void str_substr(char *buffer, size_t buflen, char const *source, int len);
char * readfile(char *filename);
char * str_removeFirstCharFromString(char* str);
char * addBarraAString(char * str);
char * str_toLowerCase(char * str);
char * str_toUpperCase(char * str);
char * str_removeLastCharFromString(char* str);
char * str_trim(char *str);
char * str_concat(const char *s1, const char *s2);
char * str_replace(char *search, char *replace, char *subject);
char ** str_split(char* a_str, const char a_delim);

#endif /* STRINGMETHODS_H */

