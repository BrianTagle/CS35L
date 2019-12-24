#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char** argv){ //looked up online how to make a main func. with arguments
  if(argc!=3){
    fprintf(stderr, "Incorrect number of arguements, found %d, but exactly 3 requried\n", argc);
    exit(1);
  }
  char* from = argv[1];
  char* to = argv[2];
  size_t F = strlen(from);
  size_t T = strlen(to);
  if(T!=F){
    fprintf(stderr, "Arguments \"from\" and \"to\" must have the same length\n");
    exit(1);
  }
  
  //printf("from:\t%s\n",from);
  //printf("length:\t%d\n", F);
  for(size_t i = 0;  i<F; i++){
    for(size_t j=i+1; j<F; j++){
      if(from[i]==from[j]){
	fprintf(stderr,"No duplicate characters allowed in argument \"from\"\n");
	exit(1);
      }
    }
  }
  char ourbuf[1];
  ssize_t errorStatus = read(0,ourbuf,1);//0 offset,location to be read to, read 1 byte
  while (errorStatus) {
    for(size_t i=0; i<F; i++){
      if(*ourbuf == from[i]){
        *ourbuf = to[i];
	break;
      }
    }
    write(1, ourbuf, 1); //,write from ourbuf, write 1 character
    //putchar(currentChar);
    //currentChar =getchar();
    errorStatus = read(0,ourbuf,1);
  }
  exit(0);
}
