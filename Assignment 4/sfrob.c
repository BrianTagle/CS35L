#include<stdio.h>
#include<stdlib.h>

int frobcmp(char const* a, char const* b)
{
  while(1)
  {
    if (*a == '\0'){ 
      a++;
      continue;
    }
    if (*b == '\0'){
      b++;
      continue;
    }
    if (*a == ' ' && *b == ' '){ //space bytes signal the end of current sequence
      return 0; // if we have reached the end of both sequences and have not returned anything yet
      // then they must be the same sequence
    }
    if (*a == ' ' || ((*a)^42) < ((*b)^42)){ //XOR bytes with 42 to deobfuscate them
      return -1; //Either means that a is less than b
    }
    if (*b == ' ' || ((*b)^42) < ((*a)^42)){
      return 1; //Either mean b is less than a
    }
    a++;
    b++;
  }
}


void freeMem(char* currentWord, char** wordList, int wordListLen){//frees up allocated memory from current word and wordlist
  int k=0;
  for (int k = 0; k < wordListLen; k++)
    free(wordList[k]);
  free (wordList);
  free (currentWord);
}

//qsort needs a function of this specific return type and parameters, so I basically just made it call
//frobcmp but have the format the qsort function wants
int qsortfrobcmp(const void* a , const void* b){
  return frobcmp(*(const char**)a, *(const char**)b);
}

int main(void){
  char* currentSequence; //pointer to the current word
  char** wordList = NULL; //pointer array that will point to pointers of the words
  int wordLen = 0; // length of the current word.
  int wordListLen = 0; //length of wordList array
  
  currentSequence = (char*)malloc(sizeof(char)); //allocate space for one byte.
  if (currentSequence == NULL){ // checking for malloc error
    fprintf(stderr, "malloc error\n");
    exit(1);
  }


  currentSequence[wordLen] = getchar(); // getting the first character from stdin. 
  if (ferror(stdin)) { //error found, so exit
    freeMem(currentSequence, wordList, wordListLen);
    exit(1);
  }
  if((!ferror(stdin) && currentSequence[wordLen] != EOF)){
  do{
    char next = getchar(); //getchar stores the next char from stdin
    if (ferror(stdin)){
      freeMem(currentSequence, wordList, wordListLen);
      exit(1);
    }
        
    if (currentSequence[wordLen] == 32){ //space character mean end of current word/start of new word
      char** tempPtrPtr = realloc(wordList, wordListLen*sizeof(char*) + sizeof(char*));
      if (tempPtrPtr == NULL){
	//freememory because realloc failed
	freeMem(currentSequence, wordList, wordListLen);
	fprintf(stderr, "memory allocation error\n");
	exit(1);
      }

      if(next==32){
      do{  //if next chars are spaces, skip over them
	next = getchar();
      }while(next==32);
      }
      wordLen=-1; //upcoming wordLen++ will make this index zero
      wordList = tempPtrPtr; //point wordlist to start of new word
      wordList[wordListLen] = currentSequence; 
      currentSequence = (char*)malloc(1);
      wordListLen++;
      
      if (next == EOF){
	break; //break while loop if next character is EOF
      }
    }
    if (next == EOF) {//need to finish the current word before reaching EOF, so add a space to signal end of current word
       next = 32;
    }
    
    wordLen++; // increment character count in the currentSequence.
    char* tempPtr = realloc(currentSequence, wordLen*sizeof(char*) + sizeof(char*));
    if (tempPtr == NULL){
      freeMem(currentSequence, wordList, wordListLen); //free memory and exit because realloc failed
      fprintf(stderr, "memory allocation error\n");
      exit(1);

    }
    currentSequence = tempPtr; //currentSequence points to succesfull realloc
    currentSequence[wordLen] = next;
    
  } while (!ferror(stdin) && currentSequence[wordLen] != EOF);
  }


  qsort(wordList, wordListLen, sizeof(char*), qsortfrobcmp);

  //print sorted list, use size_t because int may not be big enough of the array is max size

  for ( size_t i=0; i < wordListLen; i++)
  {
    for (size_t j=0; wordList[i][j] != 32; j++)
    {
      printf("%c", wordList[i][j]);
    }
    printf(" ");
  }

  //free allocated and memory and return 0, success!
  freeMem(currentSequence, wordList, wordListLen);
  return 0;
}



