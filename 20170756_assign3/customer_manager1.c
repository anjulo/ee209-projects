/*-------------------------------------
 Name: Faltamo Bereket Yisehak
 Student Id: 20170756
 Assignment 3
 file: customer_manager1.c
-------------------------------------*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/*----------------------------------------------------------------------*/
/*Uncomment and use the following code if you want */

#define UNIT_ARRAY_SIZE 64
#define GROWTH_FACTOR 64

struct UserInfo {
char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
			     // # whether the array should be expanded
			     // # or not
};

/*----------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
DB_T CreateCustomerDB(void){
  /* Uncomment and use the following implementation if you want */
  DB_T d;

  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 64 elements
  d->pArray = (struct UserInfo *)calloc(d->curArrSize,
               sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curArrSize);
    free(d);
    return NULL;
  }
  return d;
  
  //return NULL;
}
/*--------------------------------------------------------------------*/
/*
DestroyCustomerDB takes the DB_T object as its parameter. After 
checking  if the argument is NULL, it does nothing. If not, it 
goes through the array and frees the id and name of each user item, 
and then frees the DB_T object d. It returns nothing.
*/
void DestroyCustomerDB(DB_T d){
  /* fill out this function */
  //assert(0);
  if (d == NULL){
  	return;
  }
  for (int i=0;i< d->numItems;i++){
    free(d->pArray[i].id);
    free(d->pArray[i].name);
    d->pArray[i].purchase = 0;
  }
  free(d->pArray);
  free(d);
}
/*--------------------------------------------------------------------*/
/*
RegisterCustomer() has four arguments. If any of them are NULL, 
it returns -1. Otherwise, it scans the array and check if the customer
is already registered. If so, it returns -1. If then the array size 
isn't enough, it dynamically expands the size by a defined factor and 
sets them to null. If not, returns -1. It allocates the id and name 
using stdup, if not successfull returns -1 and frees any successfully
allocated one. The customer data is saved to any hole found first,
if the array is filled, it is filled on the first newly allocated 
one.
*/

int RegisterCustomer(DB_T d, const char *id, const char *name, 
											 const int purchase){
  /* fill out this function */
  //assert(0);
  if(d == NULL || id == NULL || name ==NULL || purchase <=0)
    return(-1);

  //check if the customer already registered
  for(int i=0;i< d->curArrSize;i++){
    if ((d->pArray[i].id != NULL) && (d->pArray[i].name != NULL)) {
      if((strcmp(id,d->pArray[i].id) == 0) || 
         (strcmp(name,d->pArray[i].name) == 0)){
        return(-1);
    }
  }
  }

  //expand the array and add on the numItem-th array
  if(d-> curArrSize == d-> numItems){
  //  expand the array
    struct UserInfo *pArray2 = d-> pArray;
    pArray2 = realloc(d-> pArray, (d->curArrSize+UNIT_ARRAY_SIZE)*
                                   sizeof(struct UserInfo));
    if(pArray2 == NULL)
      return(-1);
    else{
      d->pArray = pArray2;
      d->curArrSize += UNIT_ARRAY_SIZE;
    } 

    //add on the numItem-th array
    d->pArray[d->numItems].id = strdup(id);
    if(d->pArray[d->numItems].id == NULL){
      return(-1);
    }
    d->pArray[d->numItems].name = strdup(name);
    if(d->pArray[d->numItems].name == NULL){
      free(d->pArray[d->numItems].id);
      return(-1);
    } 
    d->pArray[d->numItems].purchase = purchase;
    d->numItems++;
  }

  // don't expand but look for null array element and add there
  else{
    for (int i=0; i<d->curArrSize; i++){
      if(d->pArray[i].id == NULL){
        d->pArray[i].id = strdup(id);
        //assert(d->pArray[i].id != NULL);
        if(d->pArray[i].id == NULL){
          return(-1);
        }
        d->pArray[i].name = strdup(name);
        if(d->pArray[i].name == NULL){
          free(d->pArray[i].id);
          return(-1);
        }  

        d->pArray[i].purchase = purchase;
        d->numItems++;
        break;
      }
    }
  }
  return(0);
}
/*--------------------------------------------------------------------*/

/*
UnregisterCustomerByID() has two parameters(DB_T object d and const char
pointer id). If either of the parameters are NULL, it's a filure and 
it returns -1. If not, it goes through the array looking for a used info
with the same id as the parameter. If it finds one, it frees the id and 
name, sets the purchase to 0, decrements the numnber of items by 1 and
then returns 0. If it fails to find one,  it returns -1.
*/
int UnregisterCustomerByID(DB_T d, const char *id){
  /* fill out this function */
  //assert(0);
  if( d == NULL || id == NULL)
    return(-1);
  for (int i=0; i<d->curArrSize; i++){
    if((d->pArray[i].id != NULL) && strcmp(id,d->pArray[i].id) == 0){
      free(d->pArray[i].id);
      free(d->pArray[i].name);
      //free(d->pArray[i].purchase)
      d->pArray[i].id = NULL;
      d->pArray[i].name = NULL;
      d->pArray[i].purchase = 0;
      d->numItems--;
      return 0;
    }

  }
  return (-1);
}

/*--------------------------------------------------------------------*/
/*
UnregisterCustomerByName() works in almost the same way with the above 
function UnregisterCustomerByID. It only differs by the fact that it
take a name parameter and not id. Accordingly, it looks for a user info
using the name argument. All other things are the same.
*/

int UnregisterCustomerByName(DB_T d, const char *name) {
  /* fill out this function */
  //assert(0);
  if( d == NULL || name == NULL)
    return(-1);
  for (int i=0; i<d->curArrSize; i++){
    if((d->pArray[i].name != NULL) && strcmp(name,d->pArray[i].name) == 0){
      free(d->pArray[i].id);
      free(d->pArray[i].name);
      //free(d->pArray[i].purchase)
      d->pArray[i].id = NULL;
      d->pArray[i].name = NULL;
      d->pArray[i].purchase = 0;
      d->numItems--;
      return 0;
    }

  }
  return (-1);
}
/*--------------------------------------------------------------------*/
/*
GetPurchaseByID() has two parameters(DB_T object d and const char
pointer id). If either of the parameters are NULL, it's a filure and 
it returns -1. If not, it goes through the array looking for a used info
with the same id as the parameter. If it finds one, returns its purchase
value. Otherwise, it returns -1.

*/
int GetPurchaseByID(DB_T d, const char* id){
  /* fill out this function */
  //assert(0);
  if (d == NULL || id == NULL)
    return(-1);
  for (int i=0; i<d->curArrSize; i++){
    if (d->pArray[i].id != NULL){
      if(strcmp(id,d->pArray[i].id) == 0){
       return(d->pArray[i].purchase);
      }
    }
  }
  return (-1);
}
/*--------------------------------------------------------------------*/
/*
GetPurchaseByName() works in almost the same way with the above 
function GetPurchaseByID. It only differs by the fact that it
take a name parameter and not id. Accordingly, it looks for a user info
using the name argument. All other things are the same.
*/
int GetPurchaseByName(DB_T d, const char* name){
  /* fill out this function */
  //assert(0);
  if (d == NULL || name == NULL)
    return(-1);
  for (int i=0; i<d->curArrSize; i++){
    if (d->pArray[i].name != NULL){
      if(strcmp(name,d->pArray[i].name) == 0){
        return(d->pArray[i].purchase);
      }
    }
  }
  return (-1);
}
/*--------------------------------------------------------------------*/
/*
GetSumCustomerPurchase() takes two parameters: the DB_T object d and 
function pointer fp. It checks if either of the parameters are NULL. 
If so, it returns -1. Otherwise, it goes throught the entire array and
sums all the purchase value using the function pointed by fp. It finally
returns this value.
*/
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp){
  /* fill out this function */
  //assert(0);

  if( d ==NULL || fp == NULL)
    return -1;
  int purchase_tot = 0;
  for(int i=0; i<d->numItems;i++){
    purchase_tot += fp(d->pArray[i].id ,d->pArray[i].name,
                                        d->pArray[i].purchase);
  }
  return (purchase_tot);
}