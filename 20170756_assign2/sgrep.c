/************************************
 Name: Faltamo Bereket Yisehak
 Student Id: 20170756
 Assignment 2
 file: sgrep.c
 ***********************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for skeleton code */
#include <unistd.h> /* for getopt */
#include "str.h"

#define FIND_STR        "-f"
#define REPLACE_STR     "-r"
#define DIFF_STR        "-d"

#define MAX_STR_LEN 1023

#define FALSE 0
#define TRUE  1

typedef enum {
  INVALID,
  FIND,
  REPLACE,
  DIFF
} CommandType;

/*
 * Fill out your functions here (If you need) 
 */

/*-------------------------------------------------------------------*/
/* PrintUsage()
   print out the usage of the Simple Grep Program                    */
/*-------------------------------------------------------------------*/
void PrintUsage(const char* argv0) 
{
  const static char *fmt = 
    "Simple Grep (sgrep) Usage:\n"
    "%s [COMMAND] [OPTIONS]...\n"
    "\nCOMMNAD\n"
    "\tFind: -f [search-string]\n"
    "\tReplace: -r [string1] [string2]\n"
    "\tDiff: -d [file1] [file2]\n";

  printf(fmt, argv0);
}
/*-------------------------------------------------------------------*/
/* DoFind()
  Your task:
  1. Do argument validation 
   - String or file argument length is no more than 1023
   - If you encounter a command-line argument that's too long, 
   print out "Error: argument is too long"
   
  2. Read the each line from standard input (stdin)
   - If you encounter a line larger than 1023 bytes, 
   print out "Error: input line is too long" 
   - Error message should be printed out to standard error (stderr)
   
  3. Check & print out the line contains a given string (search-string)
      
   Tips:
   - fgets() is an useful function to read characters from file. Note 
   that the fget() reads until newline or the end-of-file is reached. 
   - fprintf(sderr, ...) should be useful for printing out error
   message to standard error

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/

/*The DoFind function.
  If the length of the argument(pcSearch) is longer than 1023,   
  it prints an error message to error stream and       
  returns False. It receives input from  the standard input. 
  If the line is greater than 1023, it prints an error message
  and returns FALSE. It uses fgets function from the stdlib.        
  The function prints every line with the pcSearch 
  to the standard output stream.
*/
int DoFind(const char *pcSearch)
{
  char buf[MAX_STR_LEN + 2]; 
  int len_buf;
  int lenpc = StrGetLength(pcSearch);
  /* 
   *  Fill out your variables here //DONE!
   */
  if(lenpc > MAX_STR_LEN){
    fprintf(stderr,"rror: argument is too long");
    return FALSE;

  }
   
  /* Read the line by line from stdin, Note that this is an example */
  while (fgets(buf, sizeof(buf), stdin) != NULL) {
    /* check input line length */
    len_buf = StrGetLength(buf);
    if (len_buf > MAX_STR_LEN) {
      fprintf(stderr, "Error: input line is too long\n");
      return FALSE;
    }
    /* TODO: fill out this function */

    if( StrSearch(buf, pcSearch) != NULL){
      //cases for newline
      if(StrSearch(buf,"\n") != NULL) // already have newline
        printf("%s", buf);
      else                          // For last lines of a txt
        printf("%s\n", buf);

    }
  }
   
  return TRUE;
}
/*-------------------------------------------------------------------*/
/* DoReplace()
   Your task:
   1. Do argument validation 
      - String length is no more than 1023
      - If you encounter a command-line argument that's too long, 
        print out "Error: argument is too long"
      - If word1 is an empty string,
        print out "Error: Can't replace an empty substring"
      
   2. Read the each line from standard input (stdin)
      - If you encounter a line larger than 1023 bytes, 
        print out "Error: input line is too long" 
      - Error message should be printed out to standard error (stderr)

   3. Replace the string and print out the replaced string

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/


/*The DoReplace function .
  It takes two arguments( pcString1 and pcString2). It checks each 
  argument's length. It prints an error to the standard error stream
  and return FALSE(0) in case either of the length is greater than 
  1023. If pcString1 is an empty, it prints an error and returns 
  FALSE.Like DoFind, it also uses fgets.   
  If the line length is greater than 1023 chara cters it prints error
  to the error stream  and returns FALSE. 
  Then every occurence of pcString1 in the line it is replaced
  by pcString2 and then the updated line is printed to the standard 
  output stream TRUE is returned. If pcString1 is empty the 
  original line is printed to the standard output and TRUE is returned. 
*/
int DoReplace(const char *pcString1, const char *pcString2)
{
  /* TODO: fill out this function */  
  char buf[MAX_STR_LEN + 2]; 
  int len_buf;
  int lenpc1 = StrGetLength(pcString1);
  int lenpc2 = StrGetLength(pcString2);

  //if the length of wither strings is greater then 1023, 
  //stop with an error message 
  if(lenpc1 > MAX_STR_LEN || lenpc2 > MAX_STR_LEN){
    fprintf(stderr, "Error: argument is too long \n");
    return FALSE;
  }
  // if pcString1 is empty, stop with  an error message 
  if(lenpc1 ==0){
    fprintf(stderr, "Error: Can't replace an empty substring \n");
    return FALSE;
  }

  //printf("%s", fgets(buf,sizeof(buf), stdin));
  while(fgets(buf,sizeof(buf), stdin) != NULL){
    //printf("%s \n", buf);

    len_buf = StrGetLength(buf);
    if(len_buf > MAX_STR_LEN){
      fprintf(stderr, "Error: input line is too long \n" );
      return FALSE;
    } 

    //If string1 is not found in the line, the original line is copied
    //to stdout.

    if (StrSearch(buf,pcString1) == NULL){ // No substring found
      printf("%s", buf);
      //puts("Here!");
    }
    else{ // Substring found 
      int istr = 0, isub = 0;

      // Operate character-wise
      while (buf[istr] != '\0'){
        //printf("%d ", isub);
        if (buf[istr] != *(pcString1+isub)){ // case: No match
          if(isub == 0){ // case: No match with no previous match
            printf("%c", buf[istr]);
            //istr++;
          }
          else{ //case: No match with previous match
            for(int i=istr-isub; i<=istr; i++){
              printf("%c", buf[i]);
            }
          
          }
          isub = 0; //update isub to the first character
        }
        else{ //case: Match
          //istr++;
          if(isub == lenpc1-1){ // Full match
            printf("%s", pcString2);
            isub = 0;
            //istr++;
          }
          else{
            isub++;
          }
        }
      istr++;
      }
      //istr++;
    }
  }

  return TRUE;
}
/*-------------------------------------------------------------------*/
/* DoDiff()
   Your task:
   1. Do argument validation 
     - file name length is no more than 1023
     - If a command-line argument is too long, 
       print out "Error: arugment is too long" to stderr

   2. Open the two files
      - The name of files are given by two parameters
      - If you fail to open either file, print out error messsage
      - Error message: "Error: failed to open file [filename]\n"
      - Error message should be printed out to stderr

   3. Read the each line from each file
      - If you encounter a line larger than 1023 bytes, 
        print out "Error: input line [filename] is too long" 
      - Error message should be printed out to stderr

   4. Compare the two files (file1, file2) line by line 

   5. Print out any different line with the following format
      file1@linenumber:file1's line
      file2@linenumber:file2's line

   6. If one of the files ends earlier than the other, print out an
      error message "Error: [filename] ends early at line XX", where
      XX is the final line number of [filename].

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/

/* MY COMMENT
This function finds a difference between two files given as arguments. 
It first does argument validation to check if the argument length is
grater than a certain value. If not, opens the file. If the argument 
length is greater than 1023 or the file can't be opened, it returns 
respective error messages and closes. Then runs in a while loop line
by line of both files until it reaches either EOF or a line with number
of characters greater than 1023. If those above happen, an apropraite 
message is sent to error stream and the program is terminated. 
Otherwise, different lines corrosponding to the same line_number are
printed and the function return.

*/

int DoDiff(const char *file1, const char *file2)
{
  /* TODO: fill out this function */ 

  // argument validation: file name length is no more than 1023
  if(StrGetLength(file1) > MAX_STR_LEN || StrGetLength(file2) > MAX_STR_LEN){
    fprintf(stderr, "Error: arugment is too long \n");
    return FALSE;
  }

  // Open the files
  FILE *fl1 = fopen(file1,"r");

  // fail to open either file
  if(fl1 == NULL){
    fprintf(stderr, "Error: failed to open file %s \n", file1);
    return FALSE;
  }

   FILE *fl2 = fopen(file2,"r");
  if(fl2 == NULL){
    fprintf(stderr, "Error: failed to open file %s \n", file2);
    return FALSE;
  }

  char buf1[MAX_STR_LEN+2]; 
  char buf2[MAX_STR_LEN+2];
  int line_num = 0;

  // while one of the lines is non-NULL
  int cnt = 0;
  char *pt1, *pt2;
  int len1, len2;

  while(TRUE){


    pt1 = fgets(buf1, sizeof(buf1),fl1);
    pt2 = fgets(buf2, sizeof(buf2),fl2);

    if(pt1 == NULL && pt2 ==  NULL){
      //fprintf(stderr, "Error: %s ends early at line %d \n", file2, line_num);
      return TRUE;
    }
    
    // If one of the files ends earlier than the other
    if(pt1 != NULL && pt2 == NULL){
      fprintf(stderr, "Error: %s ends early at line %d \n", file2, 
        line_num);
      return FALSE;
    }
    if(pt1 == NULL && pt2 != NULL){
      fprintf(stderr, "Error: %s ends early at line %d \n", file1, 
        line_num);
      return FALSE;
    }
    
    // Checking the maximum line length
    if(StrGetLength(buf1) > MAX_STR_LEN){
      fprintf(stderr, "Error: input line %s is too long \n", file1);
      return FALSE;
    }
    if(StrGetLength(buf2) > MAX_STR_LEN){
      fprintf(stderr, "Error: input line %s is too long \n", file2);
      return FALSE;
    }
    line_num ++;
    
    len1 = StrGetLength(buf1);
    len2 = StrGetLength(buf2);
    //lens= min(len1, len2); // smaller

    

    if(StrCompare(pt1, pt2) != 0){
      if(((len1+1) == len2) && buf2[len1] =='\n'){
        continue;
      }
      else if((len1 == (len2+1)) && buf1[len2] =='\n'){
        continue;
      }


      if(StrSearch(buf1,"\n") != NULL) // already have newline
        printf("%s@%d:%s", file1,line_num, buf1);
      else                          // For last lines of a txt
        printf("%s@%d:%s\n", file1,line_num, buf1);

      if(StrSearch(pt2,"\n") != NULL)
      printf("%s@%d:%s", file2,line_num, buf2);
      else
        printf("%s@%d:%s\n", file2,line_num, buf2);
    }
    
    
    
    
    cnt ++;
    
  

  }

  fclose(fl1);
  fclose(fl2);

  
  return TRUE;
}
/*-------------------------------------------------------------------*/
/* CommandCheck() 
   - Parse the command and check number of argument. 
   - It returns the command type number
   - This function only checks number of argument. 
   - If the unknown function is given or the number of argument is 
   different from required number, this function returns FALSE.
   
   Note: You SHOULD check the argument rule later                    */
/*-------------------------------------------------------------------*/ 
int CommandCheck(const int argc, const char *argv1)
{
  int cmdtype = INVALID;
   
  /* check minimum number of argument */
  if (argc < 3)
    return cmdtype;
   
  /* check command type */ 
  if (StrCompare(argv1, FIND_STR) == 0) {
    if (argc != 3)
      return FALSE;    
    cmdtype = FIND;       
  }
  else if (StrCompare(argv1, REPLACE_STR) == 0) {
    if (argc != 4)
      return FALSE;
    cmdtype = REPLACE;
  }
  else if (StrCompare(argv1, DIFF_STR) == 0) {
    if (argc != 4)
      return FALSE;
    cmdtype = DIFF;
  }
   
  return cmdtype;
}
/*-------------------------------------------------------------------*/
int  main(const int argc, const char *argv[]) 
{
  int type, ret;
   
  /* Do argument check and parsing */
  if (!(type = CommandCheck(argc, argv[1]))) {
    fprintf(stderr, "Error: argument parsing error\n");
    PrintUsage(argv[0]);
    return (EXIT_FAILURE);
  }
   
  /* Do appropriate job */
  switch (type) {
  case FIND:
    ret = DoFind(argv[2]);
    break;
  case REPLACE:
    ret = DoReplace(argv[2], argv[3]);
    break;
  case DIFF:
    ret = DoDiff(argv[2], argv[3]);
    break;
  } 

  return (ret)? EXIT_SUCCESS : EXIT_FAILURE;
}
