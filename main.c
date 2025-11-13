#include "repl.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("Hello! This is the Monkey programming language!\n");
  printf("Feel free to type in commands\n");

  start_repl(stdin, stdout);

  return 0;
}
