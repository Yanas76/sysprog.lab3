#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include "lexems.h"

char *reservedWords[] = {"program", "end", "print", "implicit",
    "read", "dimension", "if", "else", "then", "end", "exit", "cycle", "do", "concurrent", "subroutine", "function",
    "module", "private", "public", "call", "none", "open", "write", "enddo", "interface", "allocate", "allocatable",
    "in", "out", "inout"};// 32 green
char *datatypes[] = {"integer", "real", "complex", "character", "logical"};// 33 yellow
char *relationalOperators2[] = {".eq.", ".ne.", ".gt.", ".lt.", ".ge.", ".le."};// 31 red 
char *logicalOperators[] = {".and.", ".or.", ".not.", ".eqv.", ".neqv."};// 31 red
char punctuationMarks[] = {',', '(', ')', '[', ']'};// 35 pink 
//char comment = '!'; // 30 grey 
char *constants[] = {".true.", ".false.", "NULL"};// //34 blue
char operatorSymbols[] = {'*', '/', ':', '=', '<', '>', '+', '-', '&'}; //31 red 
//Numbers 36 cyan 
//Text 34 blue
//Identifiers white

bool inArray(char** words, char* word, int arraySize);
bool charInArray(char* chars, char c, int arraySize);
bool isNumber(char word[], int size);
bool isHexedecimal(char word[], FILE* file);
bool isIdentifier(char word[], int size);
bool isText(char mark, FILE* file);


void displayResult(char word[], int size){

    if(inArray(reservedWords, word, sizeof(reservedWords)/sizeof(char*))){
        printf("\033[1;32m %s \033[0m", word);
        return;
    }

    if(inArray(datatypes, word, sizeof(datatypes)/sizeof(char*))){
        printf("\033[1;33m %s \033[0m", word);
        return;
    }

    if((inArray(relationalOperators2, word, sizeof(relationalOperators2)/sizeof(char*)) 
    || inArray(logicalOperators, word, sizeof(logicalOperators)/sizeof(char*)))){
        printf("\033[1;31m %s \033[0m", word);
        return;
    }

    if(inArray(constants, word, sizeof(constants)/sizeof(char*))){
        printf("\033[1;34m %s \033[0m", word);
        return;
    }

    if(isNumber(word, size)){
        printf("\033[1;36m %s \033[0m", word);
       return;
    }

    if(isIdentifier(word, size)){

        printf(" ");
        for(int i = 0; i < size; i++){
            if(word[i] != '\'') printf("\033[1;37m%c\033[0m", word[i]);
        }
        printf(" ");
        return;
    }

    printf(" <Mistake: %s> ", word);
    return;
}



int main(){

    char c;
    char word[maxNameSize] = {};
    FILE *file = fopen ("file.txt", "r");
    
    if(file == NULL){
        printf("Error! No file found");
        return 1;
    }

    fseek(file, 0L, SEEK_SET);
    int i = 0;

    do{

        bool rec = false;
        c = fgetc(file);
        if(c == ' ' || c == '\n') rec = true;

        if(('A' <= c && c <='Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9') || (c == '_') || (c == '.') || (i == 0 && c == '-')){

            word[i] = c;
            rec = true;
            if(i == 0 && c == 'Z'){

                long int current_position = ftell(file);
                if(!isHexedecimal(word, file)){
                    fseek(file, current_position, SEEK_SET);
                }else{
                    i = 0;
                    memset(word, 0, sizeof(word));
                    continue;
                }
            }

            i++;
            if(i >= maxNameSize){
                printf("Wrong format!");
                return 1;
            }
            continue;

        }else if ((i != 1 || word[0] != '-') && i != 0){    

            displayResult(word, i);
            rec = true;
            i = 0;
            memset(word, 0, sizeof(word));

        }else if(i != 0){

            printf("\033[1;31m%s \033[0m", word);
            i = 0;
            memset(word, 0, sizeof(word));
        }

        if(c == '\"'){
            isText(c, file);
            continue;
        } 

        if(c == '\''){
            isText(c, file);
            continue;
        } 

        if(c == '!'){

            while(c != '\n'){
                printf("\033[1;30m%c\033[0m", c); 
                c = fgetc(file);
            }
            printf("%c", c);
            continue;
        }

        if(charInArray(operatorSymbols, c, sizeof(operatorSymbols)/sizeof(char))){
            printf("\033[1;31m%c\033[0m", c);
            continue;
        }

        if(charInArray(punctuationMarks, c, sizeof(punctuationMarks)/sizeof(char))){
            printf("\033[1;35m%c\033[0m", c);
            continue;
        }

        if(!rec && c != EOF){
            printf(" <Not recognised: %c> ", c);
        }
           
    }while(c != EOF);

    fclose(file);
    return 0;
}



