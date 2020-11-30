#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include "StringList/StringList.h"

#define TRUE 1
#define FALSE 0


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

void listExecutables(StringList* directories){
    for(int i = 0; i < directories->length; i++){
        DIR *dir = opendir(get(directories, i)); // Open Directory

        if(dir == NULL){
            fprintf(stderr, "The directory '%s' doesn't exist\n", get(directories, i));
        }else{
            struct dirent *dirEntry;
            int fileCounter = 0;
            while ((dirEntry = readdir(dir)) != NULL){
                // Get Full FIle Path
                char* filePath = malloc(256*sizeof(char));
                strcat(filePath, get(directories, i));
                strcat(filePath, "/");
                strcat(filePath, dirEntry->d_name);

                if(isExecutableFile(filePath)){
                    printf("%s\n", filePath);
                }

                free(filePath);
            }
        }

        closedir(dir); // Close Directory
    }
}

int main(int argc, char *argv[]) {
    // Create and Initialise a StringList to hold the directories
    StringList* directories = NULL;
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

    listExecutables(directories);
    freeStringList(directories);
    return EXIT_SUCCESS;
}
