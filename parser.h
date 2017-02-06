/******************************************************************************
 * FILE: parser.h
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 * LAST REVISED: 06/fev/17
 ******************************************************************************/

#ifndef PARSER_H
#define PARSER_H

int checkIfStringHasForbiddenEnding(char* str);
char * getDomainWithOutBar();
char * getDomainWithBar();
char * completarLink(char * str);
int checkIfLinkIsSameDomain(char * str);
FILE* openLinkFile(char * arq);
void closeLinkFile(FILE* arq);
void writeLinkOnFileFinal(char *txt);
void writeLinkOnFileOtherDomain(char *txt);
void writeLinkOnFileOtherFiles(char *txt);
void writeLinkOnFileWorkSpace(char *txt);
int tratarLinha(char * linha);
int parserINIT(char * name, char * path_with_filename, char * url);
char * removeHttpFromLink(char * str);
int removeDuplicatedLinks();
int checkIfLineContainsLink(char * line);
int checkIfLineContainsSrc(char * line);
char * tratarLink(char* link);
char * tratarSrc(char* link);

//#ifdef __cplusplus
//extern "C" {
//#endif
//#ifdef __cplusplus
//}
//#endif
#endif /* PARSER_H */

