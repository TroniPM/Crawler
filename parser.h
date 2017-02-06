/******************************************************************************
 * FILE: parser.h
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 * LAST REVISED: 04/fev/17
 ******************************************************************************/

#ifndef PARSER_H
#define PARSER_H

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

//#ifdef __cplusplus
//extern "C" {
//#endif
//#ifdef __cplusplus
//}
//#endif
#endif /* PARSER_H */

