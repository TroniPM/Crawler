/******************************************************************************
 * FILE: stringmethods.h
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 * LAST REVISED: 04/fev/17
 ******************************************************************************/

#ifndef STRINGMETHODS_H
#define STRINGMETHODS_H
char* str_trim(char *str);
char* str_concat(const char *s1, const char *s2);
char* str_replace(char *search, char *replace, char *subject);
char** str_split(char* a_str, const char a_delim);
int str_contains(char * original, char * tofind);
void str_substr(char *buffer, size_t buflen, char const *source, int len);
int str_countOccurrences(char * linha, char * string2find);
int checkIfLineContainsLink(char * line);
int str_endsWith(const char *str, const char *suffix);
char * str_removeLastCharFromString(char* str);
int startsWith(const char *str, const char *prefix);


//#ifdef __cplusplus
//extern "C" {
//#endif
//#ifdef __cplusplus
//}
//#endif

#endif /* STRINGMETHODS_H */

