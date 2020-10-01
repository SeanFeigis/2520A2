//Sean Shaya Feigiss
//ID: 1096849
//All C functions for the assignment
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

struct Performance *newPerformance() {
  struct Performance *temp = malloc(sizeof(struct Performance)); //mallocs performance struct
  if (temp == NULL) { //bad malloc checking
    fprintf(stderr, "%s\n", "Malloc Failed; Exiting..." );
    exit(0);
  } else { //intializes and sets to 0 fields
    temp->reads = 0;
    temp->writes = 0;
    temp->mallocs = 0;
    temp->frees = 0;
  }
  return(temp);
}

struct Array *newArray( struct Performance *performance, unsigned int width, unsigned int capacity) {
  struct Array *temp = malloc(sizeof(struct Array)); //mallocs array struct
  if (temp == NULL) { //bad malloc checking
    fprintf(stderr, "%s\n", "Malloc Failed; Exiting..." );
    exit(0);
  }
  //assigns the fields
  temp->width = width;
  temp->capacity = capacity;
  temp->data = malloc(width*capacity); //mallocs appropriate amount of spacee
  if (temp->data == NULL) { //bad malloc checking
    fprintf(stderr, "%s\n", "Malloc Failed; Exiting..." );
    exit(0);
  }

  performance->mallocs++; //increment performance

  return(temp);
}

void readItem(struct Performance *performance, struct Array *array, unsigned int index, void *dest) {

  if (index >= array->nel) { //bad index checking
    fprintf(stderr, "%s\n", "Error, index > nel");
    exit(0);
  }

  memcpy(dest,((char *) array->data + (index * array->width)), array->width ); //copies the data to the new dest

  performance->reads++; //increment reads
}

void writeItem(struct Performance *performance, struct Array *array, unsigned int index, void *src) {

  if (index > array->nel || index >= array->capacity) { //bad index checking
    fprintf(stderr, "%s\n", "Error, index incorrect");
    exit(0);
  }

  if (index == array->nel) { //checks for exact index for new size
    array->nel++;
    //printf("increasing size\n" );
  }

  memcpy((char*)array->data + (index * array->width) , src , array->width); //copies the data to the new dest

  performance->writes++; //increments performance
}

void contract(struct Performance *performance, struct Array *array ) {
  if (array->nel == 0) { //checks for contract on empty array
    fprintf(stderr, "%s\n", "Error, nel == 0");
    exit(0);
  } else {
    array->nel--; //decrement nel
  }
}

void freeArray(struct Performance *performance, struct Array *array) {
  free(array->data); //frees data from Array
  free(array); //frees Array
  performance->frees++; //increments frees
}

void appendItem( struct Performance *performance, struct Array *array, void *src) {
  writeItem(performance, array, array->nel , src); //writes Item to the end of the list
}

void insertItem( struct Performance *performance, struct Array *array, unsigned int index, void *src) {
  //printf("NEL: %d\n", array->nel);
  void* temp = NULL;
  //printf("i:%d index:%d\n", array->nel -1, index);
  unsigned int i;

  for (i = array->nel -1; i > index ; i--) { //loops from end of list to the index, pushing the items forward
    printf("i:%d index:%d\n", i, index);
    readItem(performance, array, i, &temp);
    writeItem(performance, array, i+1, &temp);
  }
  readItem(performance, array, i, &temp); //does it one more time because loop does 1 too little iterations
  writeItem(performance, array, i+1, &temp);

  writeItem(performance, array, index, src); //writes the new item
}

void prependItem(struct Performance *performance, struct Array *array, void *src) {
  insertItem(performance, array, 0, src); //insets item to the beginning of the array
}

void deleteItem(struct Performance *performance, struct Array *array, unsigned int index) {

  void* temp = NULL;
  unsigned int i = index;
  unsigned int end = array->nel -1 ;

  while(i < end){ //runs from index to the end of the list, pushing back the items
    readItem(performance, array, i+1, &temp);
    writeItem(performance, array, i , &temp);
    i++;
  }

  contract(performance, array); //contracts the array
}


int findItem(struct Performance *performance, struct Array *array, int (*compar)(const void *, const void*), void *target) {
  void *toCompare = malloc(array->width); //creates temp compare, size of an element
  int indexOfItem = -1;
  //printf("before loop\n");
  for (unsigned int i = 0; i < array->nel; i++) { //loop the whole loop
  //  printf("loop iteration: %d\n", i);
    readItem(performance, array, i, toCompare); //reads item to toCompare
    //printf("after readItem: %d\n", i);
    if (compar(toCompare, target) == 0) { //if they are the same, break the loop and set index of item to i
      indexOfItem = i;
      break;
    }
  }

  free(toCompare);
  return(indexOfItem);
}


int searchItem( struct Performance *performance, struct Array *array, int (*compar)(const void*, const void *), void *target) {
  void *toCompare = malloc(array->width);
  int min = 0;
  int max = array->nel-1;
  int mid;
  int num;

  while (min <= max) { //algorithm follows standard binary search rules
    mid = (max+min) / 2;
    readItem(performance, array, mid, toCompare);
    num = compar(toCompare, target);

    if (num > 0 ) {
      max = mid -1;
    } else if (num < 0) {
      min = mid + 1;
    } else {
      free(toCompare);
      return(mid);
    }
  }

  free(toCompare);
  return(-1);
}
