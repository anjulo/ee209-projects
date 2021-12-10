/*-------------------------------------
 Name: Faltamo Bereket Yisehak
 Student Id: 20170756
 Assignment 1
 file: wc209.c
-------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//definition of the enum data type named enum for the states
enum State {START, OUTSIDE, INSIDE, ENTER, COMMENT, LEAVE, LEFT};

/* function prototypes for the user-defined function that 
perform different computetion in transtion.*/

enum State start(int c);
enum State outside(int c);
enum State inside(int c);
enum State enter(int c);
enum State comment(int c);
enum State leave(int c);
enum State left(int c);

//intialise the global variables
enum State dfastate = START;  			// intial default state
enum State lstate = START; 				// traces the last state 
int nlines = 0, nwords  = 0, nchars = 0;
int c; 									// for input character
int n_comment = 0;  					// traces the number of 
										// line a comment starts

/*--------------------------------------------------------------------

The main function first gets each character. Depending on what type
it is and the current state, it determined the next state. Meanwhile,
it updates the global variables declared above to track the number of 
lines, words, and characters. Besides, it also tracks comments to 
determine the number of lines an unterminated comment starts. Finally,
it prints to output-stream if successful and to the error-stream if 
a failure.

---------------------------------------------------------------------*/


int main(){

	while((c = getchar()) != EOF){

		if (dfastate ==  START){
			//printf("Hello \n ");

			nlines = 1;
			dfastate = start(c);
			lstate = START;
		}
		else if (dfastate == OUTSIDE){
			dfastate = outside(c);
			lstate = OUTSIDE;
		}
		else if (dfastate == INSIDE){
			dfastate = inside(c);
			lstate = INSIDE;
		}

		else if( dfastate == ENTER){
			dfastate = enter(c);
			lstate = ENTER;
		}
		else if ( dfastate == COMMENT){
			dfastate = comment(c);
			lstate = COMMENT;
		}
		else if ( dfastate == LEAVE){
			dfastate = leave(c);
			lstate = LEAVE;
		} 
		else{
			dfastate =left(c);
			lstate = LEFT;
		}
	}

	if (dfastate == ENTER){
		nchars ++;
		if(lstate == START || lstate == OUTSIDE || lstate == LEFT){
			nwords ++;
		}
	}

	// Output here
	if (c == EOF && (dfastate == COMMENT || dfastate == LEAVE)){
		fprintf(stderr, "Error: line %d: unterminated comment\n", n_comment);

		return EXIT_FAILURE;
	}
	else{
		printf("%d %d %d\n", nlines, nwords, nchars);
	}

	return EXIT_SUCCESS;

}

/* ------------------------------
This function handles when the input stram begins. It updates the
dfastate based on the character while updating the variables as 
the designated on the dfa.

nwords -- number of words
nchars -- number of characters
nlines -- number of lines 
lstate -- the last state
-------------------------------*/

enum State start(int c){
	if(isspace(c)){
		dfastate = OUTSIDE;
		nchars ++;
		if (c == '\n'){
			nlines ++;
		}
	}
	else if ( c == '/'){
		dfastate = ENTER;
		//nchars ++;
		//nwords ++;

	}
	else{
		dfastate = INSIDE;
		nwords ++;
		nchars ++;
	} 

	return dfastate;
}


/* ------------------------------
This function handles when the input stram begins. It updates the
dfastate based on the character while updating the variables as 
the designated on the dfa.

nwords -- number of words
nchars -- number of characters
nlines -- number of lines 
lstate -- the last state
-------------------------------*/

enum State outside(int c){
	if(isspace(c)){
		dfastate = OUTSIDE;
		nchars ++;
		if (c == '\n'){
			nlines ++;
		}
	}
	else if ( c == '/'){
		dfastate = ENTER;
	}
	else{
		dfastate = INSIDE;
		nchars ++;
		nwords ++;
	} 

	return dfastate;
}


/* ------------------------------
This function handles when the input stram begins. It updates the
dfastate based on the character while updating the variables as 
the designated on the dfa.

nwords -- number of words
nchars -- number of characters
nlines -- number of lines 
lstate -- the last state
-------------------------------*/

enum State inside(int c){
	if(isspace(c)){
		dfastate = OUTSIDE;
		nchars ++;
		if (c == '\n'){
			nlines ++;
		}
	}
	else if ( c == '/'){
		dfastate = ENTER;
		//nchars ++;
	}
	else{
		dfastate = INSIDE;
		nchars ++;
	}

	return dfastate;
}


/* ------------------------------
This function handles when the input stram begins. It updates the
dfastate based on the character while updating the variables as 
the designated on the dfa.

nwords -- number of words
nchars -- number of characters
nlines -- number of lines 
lstate -- the last state
-------------------------------*/

enum State enter(int c){

	//printf("HERE!");
	if(isspace(c)){
		nchars ++;
		if (c == '\n'){
			nlines ++;
		}
		if(lstate == START || lstate == OUTSIDE || lstate == LEFT){
			nwords ++;
			nchars ++;
		}
		else if( lstate == INSIDE || lstate == ENTER){
			nchars ++;
		}
		dfastate = OUTSIDE;
	}
	else if( c == '*'){
		nchars ++;
		dfastate = COMMENT;
		n_comment = nlines;
	}
	else if( c == '/'){
		//printf("Here! \n");
		nchars ++;
		if (lstate == START|| lstate == OUTSIDE || lstate == LEFT){
			nwords++;
		}
		dfastate = ENTER;

	}	
	else{
		//printf("HERE!");
		nchars ++;
		if (lstate == START || lstate == OUTSIDE || lstate == LEFT){
			nwords ++;
			nchars ++;
		}
		else if(lstate == INSIDE || lstate == ENTER){
			nchars ++;
		}
		dfastate = INSIDE;
	}

	return dfastate;	

}


/* ------------------------------
This function handles when the input stram begins. It updates the
dfastate based on the character while updating the variables as 
the designated on the dfa.

nwords -- number of words
nchars -- number of characters
nlines -- number of lines 
lstate -- the last state
-------------------------------*/

enum State comment(int c){

	if (c == '*'){
		dfastate = LEAVE;
	}
	else{
		dfastate  = COMMENT;
		if (c == '\n'){
			nlines ++;
			nchars ++;
		}
	}

	return dfastate;
}


/* ------------------------------
This function handles when the input stram begins. It updates the
dfastate based on the character while updating the variables as 
the designated on the dfa.

nwords -- number of words
nchars -- number of characters
nlines -- number of lines 
lstate -- the last state
-------------------------------*/

enum State leave(int c){
	if (c == '/'){
		dfastate = LEFT;

	}
	else if(c == '*'){
		dfastate = LEAVE;
	}
	else {
		dfastate = COMMENT;
		if (c == '\n'){
			nlines ++;
			nchars ++;
		}
	}

	return dfastate;

}


/* ------------------------------
This function handles when the input stram begins. It updates the
dfastate based on the character while updating the variables as 
the designated on the dfa.

nwords -- number of words
nchars -- number of characters
nlines -- number of lines 
lstate -- the last state
-------------------------------*/

enum State left(int c){
	if (c == '/'){
		dfastate = ENTER;
	}
	else if (isspace(c)){
		dfastate = OUTSIDE;
		nchars ++;
		if (c == '\n'){
			nlines ++;
		}
	}
	else{
		dfastate = INSIDE;
		nchars ++;
		nwords ++;
	}

	return dfastate;
}
