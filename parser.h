/******************************************************************************
 * FILE: parser.h
 * DESCRIPTION:
 *   Crawler to map whole website (webpages, imagens, style files, etc).
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/

#ifndef PARSER_H
#define PARSER_H

int checkIfStringHasForbiddenEnding(char* str);
int tratarLinha(char * linha);
int checkIfLineContainsLink(char * line);
int checkIfLineContainsSrc(char * line);
int checkIfLinkIsSameDomain(char * str);
int checkIfLinkWasDownloaded(char * link);
int checkIfLinkHasAnchor(char * str);
void removeDuplicatedLinks();
void removeDuplicatedLinksFolder();
void enumerateAndSave();
void closeLinkFile(FILE* arq);
void writeLinkOnFileFinal(char *txt);
void writeLinkOnFileOtherDomain(char *txt);
void writeLinkOnFileOtherFiles(char *txt);
void writeLinkOnFileWorkSpace(char *txt);
void writeLinkOnFileDownloaded(char *txt);
char * parserINIT(char * name, char * path_with_filename, char * url);
char * removerNiveisDiferentes(char * txt);
char * getDomain();
char * getDomainWithOutBar();
char * getDomainWithBar();
char * completarLink(char * str);
char * tratarLink(char* link);
char * tratarSrc(char* link);
char * removeHttpFromLink(char * str);
FILE * openLinkFile(char * arq);
char ** getDomainAndLevels();


#endif /* PARSER_H */

