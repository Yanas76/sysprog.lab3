#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include "lexems.h"


bool inArray(char** words, char* word, int arraySize){

    for(int i = 0; i < arraySize; i++){
        if(!strcmp(words[i], word)) return true;
    }
    return false;
}

bool charInArray(char* chars, char c, int arraySize){
    for(int i = 0; i < arraySize; i++){
        if(chars[i] == c) return true;
    }
    return false;
}

bool isNumber(char word[], int size){

    int points = 0;
    if(size == 0) return false;
    for(int i = 0; i < size; i++){

        if(word[i] == '-' && i == 0) continue;
        if('0' <= word[i] && word[i] <= '9') continue;
        if(word[i] == '.' && i != size - 1 && points <= 0){

            if((word[0] == '-' && i == 1) || (word[0] != '-'  && i == 0)) return false;
            points++;
            continue; 
        } 
        return false;
    }
    return true;
}

bool isHexedecimal(char word[], FILE* file){

    int i = 1;
    char c;
    c =  fgetc(file);
    if(c != '\'') return false;
    word[i] = c;
    i++;

    while(true){

        if(i >= maxNameSize) return false;
        c =  fgetc(file);
        if(i == 2 && c == '-'){
            word[i] = c;
            i++;
            continue;
        }

        if(('0' <= c && c <= '9') || ('A' <= c && c <='F')){
            word[i] = c;
            i++;
            continue;
        }

        if(c == '\''){
            word[i] = c;
            i++;
            break;
        }

        return false;
    }

    printf("\033[1;36m %s \033[0m", word);
    return true;
}

bool isIdentifier(char word[], int size){

    int i = 0;
    if(('0' <= word[i] && word[i] <= '9') || word[i] == '.' || word[i] == '-') return false;

    for(int i = 1; i < size; i++){
        if(word[i] == '-' || word[i] == '.') return false;
    }

    return true;
}

void isText(char mark, FILE* file){

    char word[maxTextSize] = {};
    int i = 0;
    char c = mark;

    while(c != '\n' || c != EOF){

        word[i] = c;
        i++;
        if(i >= maxTextSize){
            printf( "Wrong text format!\n");
            return;
        }
        c = fgetc(file);

        if(c == mark){
            printf("\033[1;34m %s\033[0m", word); 
            break;
        } 
    }
    printf("\033[1;34m%c \033[0m", c);
    memset(word, 0, sizeof(word));
    return;
}

  