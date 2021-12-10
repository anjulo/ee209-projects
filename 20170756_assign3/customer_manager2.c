/*-------------------------------------
 Name: Faltamo Bereket Yisehak
 Student Id: 20170756
 Assignment 3
 file: customer_manager2.c
-------------------------------------*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

#define UNIT_BUCKET_SIZE 1024

enum {HASH_MULTIPLIER = 65599};

/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive. Adapted from the EE209 lecture notes. */
static int hash_function(const char *pcKey, int iBucketCount){
	if(pcKey == NULL)
        return (-1);
   	int i;
   	unsigned int uiHash = 0U;
   	for (i = 0; pcKey[i] != '\0'; i++)
    	uiHash = uiHash * (unsigned int)HASH_MULTIPLIER + (unsigned int)pcKey[i];

    return (int)(uiHash % (unsigned int)iBucketCount);
}

struct UserInfo {
	char *id;
	char *name;
	int purchase; 
	struct UserInfo *nextId;
	struct UserInfo *nextName;
};

struct DB {
	struct UserInfo **ht_id;
	struct UserInfo **ht_name;
	int numBuckets;
	int numItems;
};

/*--------------------------------------------------------------------*/

/*

*/
DB_T CreateCustomerDB(void){
  	/* Uncomment and use the following implementation if you want */
  	DB_T d;

  	// create table
  	d = (DB_T) calloc(1, sizeof(struct DB));
  	if (d == NULL) {
    	fprintf(stderr, "Can't allocate a memory for DB_T\n");
    	return NULL;
	}
	d->numBuckets = UNIT_BUCKET_SIZE;
	d->ht_id = (struct UserInfo **)calloc(d->numBuckets,sizeof(struct UserInfo));
	if (d->ht_id == NULL) {
    	fprintf(stderr, "Can't allocate a memory for array of size %d\n", d->numBuckets);
    	d->numBuckets = 0;
    	free(d);
    	return NULL;
    }

	d->ht_name = (struct UserInfo **)calloc(d->numBuckets,sizeof(struct UserInfo));
	if (d->ht_name == NULL) {
    	fprintf(stderr, "Can't allocate a memory for array of size %d\n", d->numBuckets);
    	free(d->ht_id);
    	d->numBuckets = 0;
    	free(d);

    	return NULL;
    }

  	return d;
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

	for(int i=0; i<d->numBuckets; i++){
		struct UserInfo *p, *pTemp;
		for(p = d->ht_id[i]; p!=NULL; p=pTemp){
			
			pTemp = p->nextId;
			free(p->id);
			free(p->name);
			p->purchase = 0;
			free(p);
		}
		//free(pTemp);
	}  	

	free(d->ht_id);
	free(d->ht_name);
	free(d);
} 

/*--------------------------------------------------------------------*/
/*
RegisterCustomer() has four arguments. If any of them are NULL, 
it returns -1. Otherwise, it scans the hash tables and linked lists
using the hash function specified and checks if the 
customer is already registered. If so, it returns -1. If then the 
customer element number reaches  three fouth of the hash table
butkect number,  it dynamically expands to twice its size and 
customers are redsributed. 
It allocates the  id and name using stdup, if not successfull returns 
-1 and frees any successfully allocated one. 
The customer data is saved to the first element of the linked list
*/
int RegisterCustomer(DB_T d, const char *id, const char *name, const int purchase){
	/* fill out this function */


	//assert(0);

	// check if it failure case
	if(d == NULL || id == NULL || name ==NULL || purchase <=0)
    	return(-1);

    int hn_id = hash_function(id, d->numBuckets);
    int hn_name = hash_function(name, d->numBuckets);
    //check if the customer already registered 
    struct UserInfo *i;   
    for (i = d->ht_id[hn_id]; i != NULL; i = i->nextId){
    	if (i->id != NULL && strcmp(id,i->id) == 0)
    		return (-1);
    }
    for (i = d->ht_name[hn_name]; i != NULL; i = i->nextName){
    	if (i->name != NULL && strcmp(name,i->name) == 0)
    		return (-1);
    }

    //hash table expansion
    if((d->numBuckets < 1048576) && (d->numItems >= 0.75*d->numBuckets)){
    	struct UserInfo **ht_id_new = calloc(2*d->numBuckets, sizeof(struct UserInfo *));
    	if(ht_id_new == NULL){
    		return (-1);
    	}
    	struct UserInfo **ht_name_new = calloc(2*d->numBuckets, sizeof(struct UserInfo *));
		if(ht_name_new == NULL){
			free(ht_id_new);
			return (-1);
		}

		int len = d->numBuckets;
		d->numBuckets = 2*d->numBuckets;

		struct UserInfo *j; 
		int hn_id_temp,hn_name_temp;
		for(int ind=0; ind<len; ind++){
			for(j = d->ht_id[ind]; j != NULL; j = j->nextId){
				if(j->id != NULL){
					hn_id_temp = hash_function(j->id,d->numBuckets);
					d->ht_id[ind] = j->nextId;
					j->nextId = ht_id_new[hn_id_temp];
					ht_id_new[hn_id_temp] = j;
				}
			}
			for(j = d->ht_name[ind]; j != NULL; j = j->nextName){
				if(j->name != NULL){
					hn_name_temp = hash_function(j->name,d->numBuckets);
					d->ht_name[ind] = j->nextName;
					j->nextName = ht_name_new[hn_name_temp];
					ht_name_new[hn_name_temp] = j;
				}
			}
		}

		free(d->ht_id);
		free(d->ht_name);

		d->ht_id =  ht_id_new;
		d->ht_name = ht_name_new;

    }


    // finally register the god-damned customer
    struct UserInfo *ptr_cus = calloc(1,sizeof(struct UserInfo));
    if(ptr_cus == NULL)
    	return(-1);

    ptr_cus->id = strdup(id);
    if(ptr_cus->id == NULL){
    	free(ptr_cus);
    	return(-1);
    }

	ptr_cus->name = strdup(name);
	if(ptr_cus->name == NULL){
		free(ptr_cus->id);
		free(ptr_cus);
		return(-1);
	}
	ptr_cus->purchase = purchase;

	ptr_cus->nextId = d->ht_id[hn_id];
	d->ht_id[hn_id] = ptr_cus;

	ptr_cus->nextName = d->ht_name[hn_name];
	d->ht_name[hn_name] = ptr_cus;

	d->numItems++;

	//printf("5 \n");
	return(0);
}

/*--------------------------------------------------------------------*/
/*
UnregisterCustomerByID() has two arguments. It first checks if any of 
themm are null, and if so, returns -1.Otherwise, it scans the linked
list of id hastable and tries to find matching infomation. If so,
it removes the user item fromthe linked list by unlinking it, and 
returns 0.  Otherwise, it returns -1.
*/


int UnregisterCustomerByID(DB_T d, const char *id){
	/* fill out this function */
	//assert(0);
  	if( d == NULL || id == NULL)
    	return(-1);

    int hn_id = hash_function(id, d->numBuckets);
    struct UserInfo *cus_id, *cus_id_prev;
    //if(cus_id == NULL)
    //	return(-1)
    const char *name;

    // find the name of customer and id of previous customer
    for (cus_id = d->ht_id[hn_id]; cus_id!=NULL; cus_id=cus_id->nextId){
    	if(cus_id->id != NULL){
    		if(strcmp(id,cus_id->id) == 0){
    			name = cus_id->name;
    			break;
    		}
    		else
    			cus_id_prev = cus_id;
    	}
    }

    // when can't find customer
    if(cus_id == NULL)
    	return (-1);



    struct UserInfo** addr = &d->ht_id[hn_id];
    


    int hn_name = hash_function(name, d->numBuckets);
	struct UserInfo *cus_name, *cus_name_prev;

	// find the name of previous customer
    for (cus_name = d->ht_name[hn_name]; cus_name!=NULL; cus_name=cus_name->nextName){
    	if(cus_name->name != NULL){
    		if(strcmp(name,cus_name->name) == 0)
    			break;
    		else
    			cus_name_prev = cus_name;
    	}
    }
    struct UserInfo** addr2 = &d->ht_name[hn_name];
    // remove from the id linked list
    if (cus_id == d->ht_id[hn_id])
    	*addr = cus_id->nextId;
    else
    	cus_id_prev->nextId = cus_id->nextId;

    cus_id->nextId = NULL;

    // remove from the name linked list
    if (cus_name == d->ht_name[hn_name])
    	*addr2 = cus_name->nextName;
    else
    	cus_name_prev->nextName = cus_name->nextName;

    cus_name->nextName = NULL;

    // free all memory space
    free(cus_id->id); free(cus_id->name);free(cus_id);

    d->numItems--;

	return (0);
}

/*--------------------------------------------------------------------*/

/*
UnregisterCustomerByName() has two arguments. It first checks if any of 
themm are null, and if so, returns -1.Otherwise, it scans the linked
list of name hastable and tries to find matching infomation. If so,
it removes the user item fromthe linked list by unlinking it, and 
returns 0.  Otherwise, it returns -1.
*/

int UnregisterCustomerByName(DB_T d, const char *name){
  /* fill out this function */
	//assert(0);
  	if( d == NULL || name == NULL)
    	return(-1);

    int hn_name = hash_function(name, d->numBuckets);
    struct UserInfo *cus_name, *cus_name_prev;
    //if(cus_name == NULL)
    //	return(-1)
    const char *id;

    //printf("%p", (void*)d->ht_name[hn_name]);
    // find the id of customer and name of previous customer
    for (cus_name = d->ht_name[hn_name]; cus_name!=NULL; cus_name=cus_name->nextName){
    	if(cus_name->name != NULL){
    		if(strcmp(name,cus_name->name) == 0){
    			id = cus_name->id;
    			break;
    		}
    		else
    			cus_name_prev = cus_name;
    	}
    }

    // when can't find name of customer
    if(cus_name == NULL)
    	return (-1);

    struct UserInfo** addr2 = &d->ht_name[hn_name];

    int hn_id = hash_function(id, d->numBuckets);
	struct UserInfo *cus_id, *cus_id_prev;

	// find the id of previous customer
    for (cus_id = d->ht_id[hn_id]; cus_id!=NULL; cus_id=cus_id->nextId){
    	if(cus_id->id != NULL){
    		if(strcmp(id,cus_id->id) == 0)
    			break;
    		else
    			cus_id_prev = cus_id;
    	}
    }

    struct UserInfo** addr = &d->ht_id[hn_id];

    // remove from the name linked list
    if (cus_name == d->ht_name[hn_name])
    	*addr2 =  cus_name->nextName;
    else
		cus_name_prev->nextName = cus_name->nextName;
    
    cus_name->nextName = NULL;

    // remove from the id linked list
    if (cus_id == d->ht_id[hn_id])
    	*addr = cus_id->nextId;
    else
    	cus_id_prev->nextId = cus_id->nextId;
    cus_id->nextId = NULL;

    // free all memory space
    free(cus_name->name); free(cus_name->id);free(cus_name);

    d->numItems--;

	return (0);
}
/*--------------------------------------------------------------------*/

/*
GetPurchaseByID has two arguments. If any of them are NULL, it returns
-1. Otherwise, it looks through the id hastable and linkedlist looking 
for matching user info. If it finds, it returns the purchase information.
Otherwise, it returns -1.

*/
int GetPurchaseByID(DB_T d, const char* id){
	/* fill out this function */
	//assert(0);
	if (d == NULL || id == NULL)
    	return(-1);

    struct UserInfo *p;
    int h = hash_function(id, d->numBuckets);
    for(p = d->ht_id[h]; p!=NULL; p=p->nextId){
    	if(strcmp(id,p->id) == 0)
    		return p->purchase;
    }
	return (-1);
}

/*--------------------------------------------------------------------*/
/*
GetPurchaseByName has two arguments. If any of them are NULL, it returns
-1. Otherwise, it looks through the name hastable and linkedlist looking 
for matching user info. If it finds, it returns the purchase information.
Otherwise, it returns -1.

*/
int GetPurchaseByName(DB_T d, const char* name){
 	 /* fill out this function */
  	//assert(0);
	if (d == NULL || name == NULL)
    	return(-1);

    struct UserInfo *p;
    int h = hash_function(name, d->numBuckets);
    for(p = d->ht_name[h]; p!=NULL; p=p->nextName){
    	if(strcmp(name,p->name) == 0)
    		return p->purchase;
    }	
  return (-1);
}

/*--------------------------------------------------------------------*/

/*
GetSumCustomerPurchase() has two arguments. One the DB_T object and 
other the FUNCPTR_T object. If any of them are NULL, it returns -1.
Otherwise, it scans the id hastable and sums the purchase information
returnes. Then returns it.
*/
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp){
	/* fill out this function */
	//assert(0);
	if( d ==NULL || fp == NULL)
		return -1;

	int purchase_tot = 0;

	for(int i=0; i<d->numBuckets; i++){
		struct UserInfo *p;
		for(p = d->ht_id[i]; p!=NULL; p=p->nextId){
			purchase_tot += fp(p->id ,p->name, p->purchase);
		}

	}

  	return (purchase_tot);
}