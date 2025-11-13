#include <stdio.h>
#include <stdlib.h>
#include "repl.h"

int main() {
    printf("Hello! This is the Monkey programming language!\n");
    printf("Feel free to type in commands\n");
    
    start_repl(stdin, stdout);
    
    return 0;
}