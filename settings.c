/******************************************************************************
 * FILE: settings.c
 * DESCRIPTION:
 *   Crowler para obter todos os links (.html/.htm) do código fonte da página.
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "stringmethods.h"
#include "settings.h"

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

/*SETTEBLE PARAMETERS FORM COMMAND LINE*/
int ERASE_WORKSPACE_FOLDER = 1;
int LEVEL_ALLOWED = 5;

int customExtensions = 0;

//SEMPRE ADICINOAR EXTENSOES NO ARRAY ALL E NO ARRAY "FIXO"
int EXPLICIT = 0;
char * EXTENSIONS_ALL[] = {".html", ".htm", ".rb", ".rhtml", ".dll", ".cfm", ".cgi", ".svg", ".py", "jhtml", ".xhtml", ".swf", ".asp", ".aspx", ".css", ".js", ".xml", ".ico", ".jpg", ".jpeg", ".png", ".csp", ".do", ".jsf", ".jspx", ".php", ".gif"};
char * EXTENSIONS_PROHIBITED[] = {".rb", ".rhtml", ".dll", ".cfm", ".cgi", ".svg", ".py", "jhtml", ".xhtml", ".swf", ".asp", ".aspx", ".css", ".js", ".xml", ".ico", ".jpg", ".jpeg", ".png", ".csp", ".do", ".jsf", ".jspx", ".php", ".gif"};
char ** EXTENSIONS_PROHIBITED_CUSTOM;
char * EXTENSIONS_ALLOWED[] = {".html", ".htm"};
char ** EXTENSIONS_ALLOWED_CUSTOM;

char ** getExtensionsAllowed() {
    if (!customExtensions)
        return EXTENSIONS_ALLOWED;
    else
        return EXTENSIONS_ALLOWED_CUSTOM;
}

void setExtensionsAllowed(char ** newArray) {
    customExtensions = 1;
    int i;
    for (i = 0; *(newArray + i); i++) {
    }
    EXTENSIONS_ALLOWED_CUSTOM = malloc(i * sizeof (char*));

    for (i = 0; *(newArray + i); i++) {
        EXTENSIONS_ALLOWED_CUSTOM[i] = *(newArray + i);
    }

    //SEMPRE IRÁ ADICIONAR AS EXTENSÕES RESTANTES AO ARRAY DE EXTENSÕES PROIBIDAS
    setExtensionsProhibited();
}

char ** getExtensionsProhibited() {
    if (!customExtensions)
        return EXTENSIONS_PROHIBITED;
    else
        return EXTENSIONS_PROHIBITED_CUSTOM;
}

void setExtensionsProhibited() {
    int i, j, index = 0, tamFinal = (sizeof (EXTENSIONS_ALL) / sizeof (char *)) - (sizeof (getExtensionsAllowed()) / sizeof (char *));
    int tamAll = (sizeof (EXTENSIONS_ALL) / sizeof (char *));


    EXTENSIONS_PROHIBITED_CUSTOM = malloc(tamFinal * sizeof (char*));
    for (i = 0; i < tamAll; i++) {

        char ** extensoes = getExtensionsAllowed();
        int boolean = 1;

        for (j = 0; *(extensoes + j); j++) {

            if (!str_equals(EXTENSIONS_ALL[i], *(extensoes + j))) {
                boolean = 1;
            } else {
                boolean = 0;
                break;
            }
        }

        if (boolean) {
            EXTENSIONS_PROHIBITED_CUSTOM[index++] = EXTENSIONS_ALL[i];
        }
    }
}