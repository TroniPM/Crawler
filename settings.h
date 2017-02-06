/******************************************************************************
 * FILE: settings.c
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 * LAST REVISED: 06/fev/17
 ******************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H
//char * EXTENSIONS_PROHIBITED[]; // = {"css", "js", "xml", "ico", "jpg", "png", "csp", "do", "jsf", "php", "gif"};
//char * EXTENSIONS_ALLOWED[]; // = {"html", "htm"};
char * workspace_main;
char * workspance_links;
int PRINT_LINKS_FOUND;
int USE_LOCAL_INDEX_HTML;
char *URL_DOMAIN;
int ISDEBUG;

int SAVE_LINKS_OTHERDOMAINS;
int SAVE_LINKS_OTHERFILES;
#endif /* SETTINGS_H */

