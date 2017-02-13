/******************************************************************************
 * FILE: settings.c
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

char ** getExtensionsAllowed();
char ** getExtensionsProhibited();
char * workspace_main;
char * workspance_links;
char * FILENAME_LINKS;
char * FILENAME_OTHERFILES;
char * FILENAME_OTHERDOMAINS;
char * FILENAME_LINKS_DOWNLOADED;
char * FILENAME_LINKS_NOT_DOWNLOADED;
char * URL_DOMAIN;
int PRINT_LINKS_FOUND;
int USE_LOCAL_INDEX_HTML;
int ISDEBUG;
int SAVE_LINKS_OTHERDOMAINS;
int SAVE_LINKS_OTHERFILES;
int OVERIDE_OLD_FILES;
int ERASE_WORKSPACE_FOLDER;
int LEVEL_ALLOWED;
int EXPLICIT;
void setExtensionsAllowed(char ** newArray);
void setExtensionsProhibited();

#endif /* SETTINGS_H */

