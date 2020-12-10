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

/*
 * Function: isExecutableFile
 * ------------------------
 * This function checks whether the entry at a given file path is executable or not and is a file.
 *
 * @param filePath - string containing the full file path
 * @return int of TRUE (1) if it is executable, and FALSE (0) if not
 */
int isExecutableFile(const char* filePath){
    // Access File's Attributes
    struct stat fileStat;
    stat(filePath, &fileStat); // Get a stat struct for the current entry

    if(S_ISDIR(fileStat.st_mode) == 0){
        if(fileStat.st_mode & S_IXUSR) { // If entry isn't a directory and user has execute permission
            return TRUE;
        }
    }
    return FALSE;
}

/*
 * Function: listExecutables
 * ------------------------
 * This function is run by each worker thread. It loops through the entries in the given directory and checks if the
 * entry is a file, and then whether it is executable. If this is the case, the full filepath is printed.
 *
 * @param ws - void pointer to the threads Workspace struct
 * @return void pointer
 */
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

/*
 * Function: listExecsFromDirectories
 * ------------------------
 * Takes a list of directories, and creates a new thread for each one. Each thread is tasked with displaying the
 * executable files within its given directory
 *
 * @param list - Pointer to a string list struct containing directories paths
 * @return int of how the program exited.
 */
void listExecsFromDirectories(StringList* list){
    // Initialise arrays for the threads
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
        pthread_join(workerThreads[i], NULL); // Join each thread in order of creation
    }
}

/*
 * Function: main
 * ------------------------
 * Contains the main loop.
 *
 * @param argc - number of arguments the user enters
 * @param argv - array of the directories that should be searched through, either seperated by spaces or colons
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
