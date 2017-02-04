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

void openLinkFile();
void closeLinkFile();
void writeLinkOnFile(char *txt);
void tratarLinha(char * linha);
int parserINIT(char * filename);

extern char* EXTENSIONS_PROHIBITED;
extern char* EXTENSIONS_ALLOWED;

//#ifdef __cplusplus
//extern "C" {
//#endif
//#ifdef __cplusplus
//}
//#endif
#endif /* PARSER_H */

