//
// Created by JamesBurling on 27/11/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StringList.h"

StringList* initStringList(){
    StringList* stringList = malloc(sizeof(StringList));
    stringList->list = (char**) malloc(0);
    stringList->length = 0;
    return stringList;
}

void add(StringList* stringList, char* string){
    // Re-allocates the list array so that its length is incremented by one.
    stringList->list = (char**) realloc(stringList->list, sizeof(char*) * ++stringList->length);
    // sets the value of the last element in the list array equal to string.
    *(stringList->list + (stringList->length - 1)) = string;
}

char* get(StringList* stringList, int index){
    if(index < stringList->length) {
        return stringList->list[index];
    }else{
        return NULL;
    }
}

int size(StringList* stringList){
    return stringList->length;
}

StringList* splitString(char* string, char delimiter){
    StringList* stringList = initStringList();

    char* dir = strtok(string, &delimiter);
    while(dir != NULL){
        add(stringList, dir);
        dir = strtok(NULL, &delimiter);
    }

    return stringList;
}

void freeStringList(StringList* stringList){
    free(stringList->list);
    free(stringList);
}

void printStringList(StringList* stringList){
    printf("[");
    for(int i = 0; i < stringList->length; i++){
        printf("%s", get(stringList, i));
        if(i != stringList->length -1){
            printf(", ");
        }
    }
    printf("]\n");
}
