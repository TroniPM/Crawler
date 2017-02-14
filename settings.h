/******************************************************************************
 * FILE: settings.h
 * DESCRIPTION:
 *   Crowler to map whole website (webpages, imagens, style files, etc).
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
char * FILENAME_EMAIL;
char * FILENAME_OTHERFILES;
char * FILENAME_OTHERDOMAINS;
char * FILENAME_LINKS_DOWNLOADED;
char * FILENAME_LINKS_NOT_DOWNLOADED;
char * URL_DOMAIN;
int getExtensionsAllowedSize();
int getExtensionsProhibitedSize();
int PRINT_LINKS_FOUND;
int USE_LOCAL_INDEX_HTML;
int ISDEBUG;
int SAVE_LINKS_OTHERDOMAINS;
int SAVE_LINKS_OTHERFILES;
int OVERIDE_OLD_FILES;
int ERASE_WORKSPACE_FOLDER;
int LEVEL_ALLOWED;
int EXPLICIT;
int CURRENT_LEVEL;
void setExtensionsAllowed(char ** newArray, int tamanho);
void setExtensionsProhibited(int tamanho);

#endif /* SETTINGS_H */

