/******************************************************************************
 * FILE: parser.h
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/

#ifndef PARSER_H
#define PARSER_H

int checkIfStringHasForbiddenEnding(char* str);
int tratarLinha(char * linha);
int parserINIT(char * name, char * path_with_filename, char * url);
int checkIfLineContainsLink(char * line);
int checkIfLineContainsSrc(char * line);
int checkIfLinkIsSameDomain(char * str);
void removeDuplicatedLinks();
void enumerateAndSave();
void closeLinkFile(FILE* arq);
void writeLinkOnFileFinal(char *txt);
void writeLinkOnFileOtherDomain(char *txt);
void writeLinkOnFileOtherFiles(char *txt);
void writeLinkOnFileWorkSpace(char *txt);
char * getDomainWithOutBar();
char * getDomainWithBar();
char * completarLink(char * str);
char * tratarLink(char* link);
char * tratarSrc(char* link);
char * removeHttpFromLink(char * str);
FILE * openLinkFile(char * arq);

#endif /* PARSER_H */

