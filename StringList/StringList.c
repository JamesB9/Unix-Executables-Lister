//
// Created by JamesBurling on 27/11/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StringList.h"

/*
 * Function: initStringList
 * ------------------------
 * Creates and initialises a new StringList struct on the heap and returns a pointer to it
 *
 * @return pointer to a StringList struct
 */
StringList* initStringList(){
    StringList* stringList = malloc(sizeof(StringList));
    stringList->list = (char**) malloc(0);
    stringList->length = 0;
    return stringList;
}

/*
 * Function: add
 * ------------------------
 * Function to add a new string to an existing StringList struct
 *
 * @param stringList - pointer to a StringList struct to add the string to
 * @param string - array of chars to add to the StringList
 */
void add(StringList* stringList, char* string){
    // Re-allocates the list array so that its length is incremented by one.
    stringList->list = (char**) realloc(stringList->list, sizeof(char*) * ++stringList->length);
    // sets the value of the last element in the list array equal to string.
    *(stringList->list + (stringList->length - 1)) = string;
}

/*
 * Function: get
 * ------------------------
 * Returns the string at a given position in a StringList struct
 *
 * @param stringList - pointer to a StringList struct to use
 * @param index - int of the position of the string to retrieve
 *
 * @return the string at position index, or null if invalid index
 */
char* get(StringList* stringList, int index){
    if(index < stringList->length) {
        return stringList->list[index];
    }else{
        return NULL;
    }
}

/*
 * Function: size
 * ------------------------
 * Returns the number of strings stored in the StringList struct
 *
 * @return int of the number of strings in the struct
 */
int size(StringList* stringList){
    return stringList->length;
}

/*
 * Function: splitString
 * ------------------------
 * Creates a new StringList struct on the heap. It splits the 'string' parameter into substrings by the delimiter
 * parsed as a parameter. It adds the substrings to the StringList struct and returns a pointer to this struct.
 *
 * @param string - array of chars
 * @param delimiter - char to use to split the string parameter
 *
 * @return pointer a StringList struct
 */
StringList* splitString(char* string, char delimiter){
    StringList* stringList = initStringList();

    char* dir = strtok(string, &delimiter);
    while(dir != NULL){
        add(stringList, dir);
        dir = strtok(NULL, &delimiter);
    }

    return stringList;
}

/*
 * Function: freeStringList
 * ------------------------
 * Frees the memory the StringList struct uses on the heap
 *
 * @param stringList - the struct to use and free
 */
void freeStringList(StringList* stringList) {
    free(stringList->list);
    free(stringList);
}

/*
 * Function: printStringList
 * ------------------------
 * Prints the contents of a StringList struct in a nice formatted manner to the stdout
 *
 * @param stringList - the struct to print
 */
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
