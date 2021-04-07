//
// Created by James on 27/11/2020.
//

#ifndef OS3_COURSEWORK_STRINGLIST_H
#define OS3_COURSEWORK_STRINGLIST_H

typedef struct StringListStruct {
    char** list;
    int length;
}StringList;

StringList* initStringList();

void add(StringList* stringList, char* string);

char* get(StringList* stringList, int index);

int size(StringList* stringList);

StringList* splitString(char* string, char delimiter);

void freeStringList(StringList* stringList);

void printStringList(StringList* stringList);

#endif //OS3_COURSEWORK_STRINGLIST_H
