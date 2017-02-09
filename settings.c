/******************************************************************************
 * FILE: settings.c
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int USE_LOCAL_INDEX_HTML = 0;

char * workspace_main = "workspace_crowler/";
char * workspance_links = "links/";

int PRINT_LINKS_FOUND = 0;

int SAVE_LINKS_OTHERDOMAINS = 1;
int SAVE_LINKS_OTHERFILES = 1;

char * FILENAME_LINKS = "links_valid.txt";
char * FILENAME_OTHERFILES = "links_otherFiles.txt";
char * FILENAME_OTHERDOMAINS = "links_otherDomains.txt";
char * FILENAME_LINKS_DOWNLOADED = "downloaded.txt";
char * FILENAME_LINKS_NOT_DOWNLOADED = "not_downloaded.txt";

int ISDEBUG = 1;

int OVERIDE_OLD_FILES = 1;
int ERASE_WORKSPACE_FOLDER = 0;