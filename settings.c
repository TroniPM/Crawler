/******************************************************************************
 * FILE: settings.c
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 * LAST REVISED: 06/fev/17
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

//char * EXTENSIONS_PROHIBITED[11] = {"css", "js", "xml", "ico", "jpg", "png", "csp", "do", "jsf", "php", "gif"};
//char * EXTENSIONS_ALLOWED[2] = {"html", "htm"};
char * workspace_main = "workspace_crowler/";
char * workspance_links = "links/";
int PRINT_LINKS_FOUND = 0;
int SAVE_LINKS_OTHERDOMAINS = 1;
int SAVE_LINKS_OTHERFILES = 1;

int USE_LOCAL_INDEX_HTML = 1;
int ISDEBUG = 1;