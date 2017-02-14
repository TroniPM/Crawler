/******************************************************************************
 * FILE: settings.c
 * DESCRIPTION:
 *   Crowler to map whole website (webpages, imagens, style files, etc).
 * AUTHOR: Paulo Mateus
 * EMAIL: paulomatew@gmail.com
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "stringmethods.h"
#include "settings.h"


char * FILENAME_LINKS = "links_valid.txt";
char * FILENAME_EMAIL = "links_email.txt";
char * FILENAME_OTHERFILES = "links_otherFiles.txt";
char * FILENAME_OTHERDOMAINS = "links_otherDomains.txt";
char * FILENAME_LINKS_DOWNLOADED = "downloaded.txt";
char * FILENAME_LINKS_NOT_DOWNLOADED = "not_downloaded.txt";
char * workspace_main = "workspace_crowler/";
char * workspance_links = "links/";
int USE_LOCAL_INDEX_HTML = 0;
int PRINT_LINKS_FOUND = 0;
int SAVE_LINKS_OTHERDOMAINS = 1;
int SAVE_LINKS_OTHERFILES = 1;
int ISDEBUG = 1;
int OVERIDE_OLD_FILES = 1;
int EXTENSION_ALLOWED_SIZE = 2;
int EXTENSION_PROHIBITED_SIZE = 42;
int customExtensions = 0;

/*SETTEBLE PARAMETERS FORM COMMAND LINE*/
int CURRENT_LEVEL = 1;
int EXPLICIT = 0;
int ERASE_WORKSPACE_FOLDER = 1;
int LEVEL_ALLOWED = 3;

//SEMPRE ADICINOAR EXTENSOES NO ARRAY ALL E NO ARRAY "FIXO" || alterar EXTENSION_PROHIBITED_SIZE e EXTENSION_ALLOWED_SIZE
char * EXTENSIONS_ALL[] = {".html", ".htm", ".php", ".rb", ".rhtml", ".dll", ".cfm", ".cgi", ".svg", ".py", "jhtml", ".xhtml", ".swf", ".asp", ".aspx", ".css", ".js", ".xml", ".ico", ".jpg", ".jpeg", ".png", ".csp", ".do", ".jsf", ".jspx", ".pdf", ".gif", ".ps", ".txt", ".shar", ".roff", ".tgz", ".zip", ".rar", ".tar", ".csv", ".exe", ".bat", ".rtf", ".doc", ".docx", ".odt", ".gz"};
char * EXTENSIONS_PROHIBITED[] = {".php", ".css", ".js", ".xml", ".ico", ".jpg", ".jpeg", ".png", ".pdf", ".gif", ".rb", ".rhtml", ".dll", ".cfm", ".cgi", ".svg", ".py", "jhtml", ".xhtml", ".swf", ".asp", ".aspx", ".csp", ".do", ".jsf", ".jspx", ".ps", ".txt", ".shar", ".roff", ".tgz", ".zip", ".rar", ".tar", ".csv", ".exe", ".bat", ".rtf", ".doc", ".docx", ".odt", ".gz"};
char ** EXTENSIONS_PROHIBITED_CUSTOM;
char * EXTENSIONS_ALLOWED[] = {".html", ".htm"};
char ** EXTENSIONS_ALLOWED_CUSTOM;

char ** getExtensionsAllowed() {
    if (!customExtensions)
        return EXTENSIONS_ALLOWED;
    else
        return EXTENSIONS_ALLOWED_CUSTOM;
}

int getExtensionsAllowedSize() {
    return EXTENSION_ALLOWED_SIZE;
}

int getExtensionsProhibitedSize() {
    return EXTENSION_PROHIBITED_SIZE;
}

void setExtensionsAllowed(char ** newArray, int tamanho) {
    //logs("setExtensionsAllowed");
    customExtensions = 1;
    EXTENSION_ALLOWED_SIZE = tamanho;
    int i;
    char ** arr = newArray;

    EXTENSIONS_ALLOWED_CUSTOM = malloc(tamanho * sizeof (char*));

    for (i = 0; i < tamanho; i++) {
        EXTENSIONS_ALLOWED_CUSTOM[i] = arr[i];
    }

    //SEMPRE IRÁ ADICIONAR AS EXTENSÕES RESTANTES AO ARRAY DE EXTENSÕES PROIBIDAS
    setExtensionsProhibited(tamanho);
}

char ** getExtensionsProhibited() {
    if (!customExtensions)
        return EXTENSIONS_PROHIBITED;
    else
        return EXTENSIONS_PROHIBITED_CUSTOM;
}

void setExtensionsProhibited(int tamanho) {
    int i, j, index = 0, boolean;
    char ** extensoes = getExtensionsAllowed();

    int tamAll = (sizeof (EXTENSIONS_ALL) / sizeof (char *));

    int tamFinal = tamAll - tamanho;
    EXTENSION_PROHIBITED_SIZE = tamFinal;

    EXTENSIONS_PROHIBITED_CUSTOM = malloc(tamFinal * sizeof (char*));

    for (i = 0; i < tamAll; i++) {
        boolean = 1;

        for (j = 0; j < tamanho; j++) {
            //printf("%s == %s\n", EXTENSIONS_ALL[i], *(extensoes + j));
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