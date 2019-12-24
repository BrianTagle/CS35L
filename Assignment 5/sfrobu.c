#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<ctype.h>
#include<string.h>
#include<limits.h>
int setf=0;
char defrobnicate(const char a){
  if (setf && a>=0 && a<=UCHAR_MAX){
    return toupper((a^42));
    }
  else{
    return ((a^42));
  }

}
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
    if (*a == ' ' || defrobnicate(*a) < defrobnicate(*b) ){ //XOR bytes with 42 to deobfuscate them
      return -1; //Either means that a is less than b
    }
    if (*b == ' ' ||  defrobnicate(*b) < defrobnicate(*a)){
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

//int main(int argc, char** argv){
  /*
  //false
  char* filebuf;
  int filebufsize=0;
  int currentfilebuf=0;//tells us what byte we are at in the buffer

  char* currentSequence; //pointer to the current word
  char** wordList = NULL; //pointer array that will point to pointers of the words
  int wordLen = 0; // length of the current word.
  int wordListLen = 0; //length of wordList array
  if(argc > 2){
    fprintf(stderr,"Inocrrect number of arguements, only option allowed is -f\n");
    exit(1);
  }
  else if(argc==2){
    if(strcmp(argv[1],"-f")==0){
      setf=1;
    }
    else{
      fprintf(stderr, "Unrecognized argument '%s', only allowed arguement is -f", argv[1]);
      exit(1);
    }
  }
  
  struct stat fileData;
  if(fstat(0,&fileData)<0){
    fprintf(stderr, "fstat encountered an error.\n");
    exit(1);
  }


  if(S_ISREG(fileData.st_mode)){ //ensure file is a regular file
    filebuf = (char*)malloc(fileData.st_size*sizeof(char));
    if(filebuf==NULL){
      fprintf(stderr,"malloc error\n");
      exit(1);
    }
    if(read(0,filebuf,fileData.st_size)<0){
      fprintf(stderr, "error with reading the file\n");
      }
    filebufsize = fileData.st_size;
  }

  currentSequence = (char*)malloc(sizeof(char)); //allocate space for one byte.
  if (currentSequence == NULL){ // checking for malloc error
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  ssize_t IOStatus;
  if(filebufsize <= currentfilebuf){
    IOStatus = read(0, currentSequence, 1);
    if(IOStatus<0){
      fprintf(stderr,"system call error with read\n");
      freeMem(currentSequence, wordList, wordListLen);
      exit(1);
    }
    else{
      currentSequence[wordLen] = filebuf[currentfilebuf];
      currentfilebuf+=1;
      IOStatus =1;
    }
  }

  // currentSequence[wordLen] = getchar(); // getting the first character from stdin. 
  // if (ferror(stdin)) { //error found, so exit
  //  freeMem(currentSequence, wordList, wordListLen);
  //   exit(1);
  // }
  
  while (IOStatus >0){
    char next; //getchar stores the next char from stdin
    if(filebufsize <= currentfilebuf){
      IOStatus = read(0, &(next), 1);
      if(IOStatus<0){
	fprintf(stderr,"system call error with read\n");
	freeMem(currentSequence, wordList, wordListLen);
	exit(1);
      }
      else{
	next = filebuf[currentfilebuf];
	currentfilebuf+=1;
       
      }
    }
    // if (ferror(stdin)){
    // freeMem(currentSequence, wordList, wordListLen);
    // exit(1);
    //}
        
    if (currentSequence[wordLen] == 32){ //space character mean end of current word/start of new word
      char** tempPtrPtr = realloc(wordList, wordListLen*sizeof(char*) + sizeof(char*));
      if (tempPtrPtr == NULL){
	//freememory because realloc failed
	freeMem(currentSequence, wordList, wordListLen);
	fprintf(stderr, "memory allocation error\n");
	exit(1);
      }

      
      while(next==32){  //if next chars are spaces, skip over them
      //	next = getchar();
	if(filebufsize <= currentfilebuf){
	  IOStatus = read(0, &(next), 1);
	  if(IOStatus<0){
	    fprintf(stderr,"system call error with read\n");
	    freeMem(currentSequence, wordList, wordListLen);
	    exit(1);
	  }
	  else{
	    next = filebuf[currentfilebuf];
	    currentfilebuf+=1;
       
	  }
	}
      }
      
      wordLen=-1; //upcoming wordLen++ will make this index zero
      wordList = tempPtrPtr; //point wordlist to start of new word
      wordList[wordListLen] = currentSequence; 
      currentSequence = (char*)malloc(1);
      wordListLen++;
      
      if (IOStatus == 0){
	break; //break while loop if next character is EOF
      }
    }
    if (IOStatus == 0) {//need to finish the current word before reaching EOF, so add a space to signal end of current word
       next = 32;
       IOStatus =1;
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
    
  } 
  


*/
int main(int argc, char** argv){
  char* currentSequence;
  char** wordList=NULL; 
  int wordLen = 0;
  int wordListLen = 0; 
  char* filebuff;
  int currentfilebuff = 0;
  int filebuffsize = 0;
  if (argc > 2)
  {
    fprintf(stderr, "incorrect number of arguments.\n");
    exit(1);
  }



  if(argc > 2){
    fprintf(stderr,"Incorrect number of arguments\n");
    exit(1);
  }
  else if(argc==2){
    if(strcmp(argv[1],"-f")==0){
      setf=1;
    }
    else{
      fprintf(stderr, "Unrecognized argument '%s', only allowed arguement is -f", argv[1]);
      exit(1);
    }
  }


  struct stat fileData;
  if (fstat(0, &fileData) != 0) //0 indicates succesful completions
  {
    fprintf(stderr, "fstat ran an error\n ");
    exit(1);
  }


  if (S_ISREG(fileData.st_mode))
  {
    filebuff = (char*)malloc(fileData.st_size * sizeof(char));  //dont need to work for a changing file size
    if (filebuff == NULL)
    {
      fprintf(stderr, "Malloc error\n");
      exit(1);
    }
    if (read(0, filebuff, fileData.st_size) < 0)
    {
      fprintf(stderr, "Read error");
      exit(1);
    }
    filebuffsize = fileData.st_size;
  }
  
  currentSequence = (char*)malloc(sizeof(char)); // give space for 1 character.
  if (currentSequence == NULL) // check malloc error
  {
    fprintf(stderr, "Malloc error\n");
    exit(1);
  }



  ssize_t IOstatus;
  /*
  if (currentfilebuff >= filebuffsize)
  {
    IOstatus = read(0, (currentSequence), 1);
    if (IOstatus < 0) // get first char with read
    {
      fprintf(stderr,"Read error");
      freeMem(currentSequence, wordList, wordListLen);
      exit(1);
    }
  }
  else {
    currentSequence[wordLen] = filebuff[currentfilebuff];
    currentfilebuff++;
    IOstatus = 1;
    }*/
  
  //while (IOstatus > 0) 
  do{
    char next;

    if (currentfilebuff >= filebuffsize){
    
      IOstatus = read(0, &(next), 1);
      if (IOstatus<0){
      
	fprintf(stderr,"Read error");
	freeMem(currentSequence, wordList, wordListLen);
	exit(1);
      }
    }
    else{
      next = filebuff[currentfilebuff];
      currentfilebuff++;
    }
    
        
    if (currentSequence[wordLen] == 32){ // if char is space, then next word
    
     
      char** tempPtrPtr = realloc(wordList, wordListLen*sizeof(char*) + sizeof(char*));
      if (tempPtrPtr == NULL){
	fprintf(stderr, "Realloc error\n");
	freeMem(currentSequence, wordList, wordListLen);
	exit(1);

      } 
      wordList = tempPtrPtr; 
      wordList[wordListLen++] = currentSequence; 
      //wordListLen++;
      wordLen = -1; 
      currentSequence = (char*)malloc(1); //space for one byte.
      
      if (IOstatus == 0){ 
	break;
      }
      while (next == 32){
      
	if (currentfilebuff >= filebuffsize){
	  IOstatus = read(0, &(next), 1);
	  if (IOstatus<0){ 
	  
	    fprintf(stderr,"Read Error");
	    freeMem(currentSequence, wordList, wordListLen);
	    exit(1);
	  }
	}
	else{
	    next = filebuff[currentfilebuff];
	    currentfilebuff++;
	}
	
      }
    } 
    else if (IOstatus == 0) { 
      next = 32;
      IOstatus = 1;
    }
    
    wordLen++; 
    char* tempPtr = realloc(currentSequence, wordLen*sizeof(char*) + sizeof(char*));
    if (tempPtr == NULL){
      freeMem(currentSequence, wordList, wordListLen);
      fprintf(stderr, "Realloc error\n");
      exit(1);

    }

    currentSequence = tempPtr; 
    currentSequence[wordLen] = next; 
    
  } while (IOstatus > 0) ;

//qsort(wordList, wordListLen, sizeof(char*), qsortfrobcmp);

  //print sorted list, use size_t because int may not be big enough of the array is max size

  qsort(wordList, wordListLen, sizeof(char*), qsortfrobcmp);

  // print sorted list with write.
  for (size_t i = 0; i < wordListLen; i++){
    for (size_t j = 0; wordList[i][j] != 32; j++){
      write(1, &wordList[i][j], 1);
    }
    write(1, " ", 1);
  }

  // free memory and exit with code 0, success!
  freeMem(currentSequence, wordList, wordListLen);
  exit(0);
/*
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
*/
}



