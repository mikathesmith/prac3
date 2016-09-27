#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "htable.h"


struct htablerec {
    int capacity;
    int numkeys;
    char **keys;
};


htable htable_new(int size) {
    int i;
    htable result = emalloc(sizeof *result); /*declare htable, allocate memory*/
    result->capacity = size;  
    result->numkeys = 0;
    result->keys = emalloc(result->capacity * sizeof result->keys[0]);
    
    for(i=0; i < size; i++){
        result->keys[i] = NULL;
    }
    
    return result;
}

static unsigned int htable_word_to_int(char *word){
    unsigned int result=0;
    while(*word != '\0'){
        result= (*word++ + 31 * result);
    }
    return result;
}



int htable_insert(htable h, char *str) {
    unsigned int val = htable_word_to_int(str);
    int rem = val % (unsigned int)h->capacity;
    int collisions=0;
    /*To find an empty cell, increment rem while its less than capacity and either not null or not
      a cell containing the same str value*/
    while(collisions < h->capacity && h->keys[rem]!= NULL  && strcmp(h->keys[rem], str) != 0){
        rem++;
        rem = rem % h->capacity;
        collisions++;
    } 
    if(h->keys[rem]== 0){ /*If the cell is empty.*/
        h->keys[rem] = emalloc((strlen(str)+1) * sizeof str[0]);
        strcpy(h->keys[rem], str);
        /*h->keys[rem] = val; Insert the value*/
        h->numkeys++; 
        return 1;
    }else{
        return 0;  /*return 0 if there is no empty space and htable is full*/
    }
}



void htable_print(htable h, FILE *stream) {
    int i;
    for (i = 0; i < h->capacity; i++) {
        fprintf(stream, "%2d %s\n", i, h->keys[i] == NULL ? "" : h->keys[i]);
    } 
}


void htable_free(htable h) {   
    int i;
    for(i=0; i < h->capacity; i++){
        free(h->keys[i]);
    }
    free(h);  
}
