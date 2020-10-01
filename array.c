#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

struct Performance *newPerformance() {
  struct Performance *temp = malloc(sizeof(struct Performance));

  if (temp == NULL) {
    fprintf(stderr, "%s\n", "Malloc Failed; Exiting..." );
    exit(0);
  } else {
    temp->reads = 0;
    temp->writes = 0;
    temp->mallocs = 0;
    temp->frees = 0;
  }

  return(temp);

}

struct Array *newArray( struct Performance *performance, unsigned int width, unsigned int capacity) {

  struct Array *temp = malloc(sizeof(struct Array));
  if (temp == NULL) {
    fprintf(stderr, "%s\n", "Malloc Failed; Exiting..." );
    exit(0);
  }

  temp->width = width;
  temp->capacity = capacity;
  temp->data = malloc(width*capacity);
  if (temp->data == NULL) {
    fprintf(stderr, "%s\n", "Malloc Failed; Exiting..." );
    exit(0);
  }

  performance->mallocs++;

  return(temp);

}

void readItem(struct Performance *performance, struct Array *array, unsigned int index, void *dest) {
  //printf("index: %d, array->nel: %d\n", index, array->nel);
  if (index >= array->nel) {
    fprintf(stderr, "%s\n", "Error, index > nel");
    //printf("nel:%d index:%d\n", array->nel, index);
    exit(0);
  }

  memcpy(dest,((char *) array->data + (index * array->width)), array->width );

  performance->reads++;
  //printf("blah\n");
}

void writeItem(struct Performance *performance, struct Array *array, unsigned int index, void *src) {

  if (index > array->nel || index >= array->capacity) {
    fprintf(stderr, "%s\n", "Error, index incorrect");
    exit(0);
  }

  if (index == array->nel) {
    array->nel++;
    //printf("increasing size\n" );
  }

  memcpy((char*)array->data + (index * array->width) , src , array->width);

  performance->writes++;

}

void contract(struct Performance *performance, struct Array *array ) {
  if (array->nel == 0) {
    fprintf(stderr, "%s\n", "Error, nel == 0");
    exit(0);
  } else {
    array->nel--;
  }
}

void freeArray(struct Performance *performance, struct Array *array) {
  free(array->data);
  free(array);
  performance->frees++;
}

void appendItem( struct Performance *performance, struct Array *array, void *src) {
  writeItem(performance, array, array->nel , src);
}

void insertItem( struct Performance *performance, struct Array *array, unsigned int index, void *src) {
  //printf("NEL: %d\n", array->nel);
  void* temp = NULL;
  //printf("i:%d index:%d\n", array->nel -1, index);
  unsigned int i;

  for (i = array->nel -1; i > index ; i--) {
    printf("i:%d index:%d\n", i, index);
    readItem(performance, array, i, &temp);
    writeItem(performance, array, i+1, &temp);
  }
  readItem(performance, array, i, &temp);
  writeItem(performance, array, i+1, &temp);

  writeItem(performance, array, index, src);

}

void prependItem(struct Performance *performance, struct Array *array, void *src) {
  insertItem(performance, array, 0, src);
}

void deleteItem(struct Performance *performance, struct Array *array, unsigned int index) {

  void* temp = NULL;
  unsigned int i = index;
  unsigned int end = array->nel -1 ;

  while(i < end){
    readItem(performance, array, i+1, &temp);
    writeItem(performance, array, i , &temp);
    i++;
  }

  contract(performance, array);
}


int findItem(struct Performance *performance, struct Array *array, int (*compar)(const void *, const void*), void *target) {
  void *toCompare = malloc(array->width);
  int indexOfItem = -1;
  //printf("before loop\n");
  for (unsigned int i = 0; i < array->nel; i++) {
  //  printf("loop iteration: %d\n", i);
    readItem(performance, array, i, toCompare);
    //printf("after readItem: %d\n", i);
    if (compar(toCompare, target) == 0) {
      indexOfItem = i;
      break;
    }
  }

  free(toCompare);
  return(indexOfItem);
}


int searchItem( struct Performance *performance, struct Array *array, int (*compar)(const void*, const void *), void *target) {
  void *toCompare;
  toCompare = malloc(array->width);
  int min = 0;
  int max = array->nel-1;
  int mid;
  int num;

  while (min <= max) {
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
