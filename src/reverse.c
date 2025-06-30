#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAXCHARS 255

void die(const char *message){
  if (errno) {
    perror(message);
  } else { 
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

void strReverse(const char* msg){
  char revString[MAXCHARS] = {};
  int numChars = strlen(msg);
  int i = {}; 
  int j = numChars;

  printf("Total Characters: %d\n", numChars);

  for(i = 0; i < numChars + 1; i++){
    revString[i] = msg[j];
    j--;
    revString[i + 1] = '\0';
    printf("%s", &revString[i]);

  }

  puts("\n");
}

int main(int argc, char* argv[]){
  if (argc < 2) die("One or more strings required.");
  
  puts("\n");
  // const char* userString = argv[1];
  
  int i = {};
  int* numArgs = &i;

  for(i = 1; argv[i] != NULL; i++){
    *numArgs = i; 
  }

  printf("%s\n", argv[0]);
  printf("Number of args: %d\n", *numArgs - 1);

  for(int j = 1; j < *numArgs; j++){
    strReverse(argv[j]);
  }

  // strReverse(argv[1]);

  return 0;

}
