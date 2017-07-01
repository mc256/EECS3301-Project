#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


char * buffer;

int main() {
    buffer = malloc(sizeof(char) * 1);
    * buffer = '\0';
    printf("%ld\n", (long)buffer);
    char * temp = realloc(buffer,sizeof(long)*sizeof(long)*sizeof(long));
    printf("%ld\n", (long)temp);
    
    //while ((c = getchar()) != EOF){
        //char * temp = realloc(buffer,strlen(buffer) + 2);
        
    //}

    bool test = false;

    printf(test ? "true" : "false");

    return 0;
}