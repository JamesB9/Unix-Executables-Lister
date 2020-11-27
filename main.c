#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#include "StringList/StringList.h"


void listExecutables(StringList* directories){
    for(int i = 0; i < directories->length; i++){
        DIR *dir = opendir(get(directories, i)); // Open Directory

        struct dirent *dirEntry;
        int fileCounter = 0;
        while ((dirEntry = readdir(dir)) != NULL){
            // Access File's Attributes
            struct stat fileStat;
            char buf[256];
            stat(realpath(dirEntry->d_name, buf), &fileStat); // Get a stat struct for the current entry

            if(S_ISDIR(fileStat.st_mode) == 0){
                //printf("FILE ");
                if(fileStat.st_mode & S_IXUSR) { // If entry isn't a directory and user has execute permission
                    //printf("EXEC ");
                    printf("%s\n", dirEntry->d_name);
                }
            }
            //printf("%s\n", dirEntry->d_name);
        }

        closedir(dir); // Close Directory
    }
}

int main(int argc, char *argv[]) {
    // Create and Initialise a StringList to hold the directories
    StringList* directories = NULL;

    // CREATING LIST OF DIRECTORIES FROM ARGUMENTS //
    if(argc == 1){
        fprintf(stderr, "You haven't entered a directory to read from\n"
                        "e.g. 'executables /etc /bin' or 'executables /etc:/bin'\n");
        return EXIT_FAILURE;
    }else if(argc == 2){
        directories = splitString(argv[1], ':');
    }else{
        directories = initStringList();
        for(int i = 1; i < argc; i++){
            add(directories, argv[i]);
        }
    }

    listExecutables(directories);
    return EXIT_SUCCESS;
}
