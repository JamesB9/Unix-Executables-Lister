#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include "StringList/StringList.h"

#define TRUE 1
#define FALSE 0
#define MAX_PATH_SIZE 256



/* Structure: Workspace
 * Holds all variables required for each worker thread.
 *
 * @variable directory   - String containing the path of the directory to search in
 */
typedef struct WorkspaceStruct{
    char* directory;
}Workspace;

int isExecutableFile(const char* filePath){
    // Access File's Attributes
    struct stat fileStat;
    stat(filePath, &fileStat); // Get a stat struct for the current entry

    if(S_ISDIR(fileStat.st_mode) == 0){
        if(fileStat.st_mode & S_IXOTH) { // If entry isn't a directory and user has execute permission
            return TRUE;
        }
    }
    return FALSE;
}

void* listExecutables(void *ws){
    Workspace *workspace = (Workspace*) ws;

    DIR *dir = opendir(workspace->directory); // Open Directory

    if(dir == NULL){
        fprintf(stderr, "The directory '%s' doesn't exist\n", workspace->directory);
    }else{
        struct dirent *dirEntry;
        int fileCounter = 0;
        while ((dirEntry = readdir(dir)) != NULL){
            // Get Full File Path
            char filePath[MAX_PATH_SIZE];
            snprintf(filePath, MAX_PATH_SIZE,"%s/%s", workspace->directory, dirEntry->d_name);

            if(isExecutableFile(filePath)){
                printf("%s\n", filePath);
            }
        }
    }

    closedir(dir); // Close Directory
    return NULL;
}


void listExecsFromDirectories(StringList* list){
    Workspace workspaces[list->length];
    pthread_t workerThreads[list->length];

    for(int i = 0; i < list->length; i++) {
        workspaces[i].directory = get(list, i);

        int status = pthread_create(&(workerThreads[i]), NULL, listExecutables, &workspaces[i]);
        if(status != 0){
            perror("Error creating Thread: ");
        }
    }

    for(int i = 0; i < list->length; i++) {
        pthread_join(workerThreads[i], NULL);
    }
}

/*
 * Function: main
 * ------------------------
 * Contains the main loop.
 *
 * @param argc - number of arguments the user enters
 * @param 
 * @return int of how the program exited.
 */
int main(int argc, char *argv[]) {
    StringList* directories;

    // CREATING LIST OF DIRECTORIES FROM ARGUMENTS //
    if(argc == 1){ // No args = Use current working directory
        directories = initStringList();
        add(directories, ".");
    }else if(argc == 2){ // 1 arg = Split dir:dir:dir by ':'
        directories = splitString(argv[1], ':');
    }else{ // >1 args = each arg is a directory
        directories = initStringList();
        for(int i = 1; i < argc; i++){
            add(directories, argv[i]);
        }
    }

    listExecsFromDirectories(directories);
    freeStringList(directories);
    return EXIT_SUCCESS;
}
