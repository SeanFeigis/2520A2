//Author: Sean Shaya Feigis
//ID: 1096849
//Header file for the assignment
struct Performance *newPerformance();
struct Array *newArray( struct Performance *performance, unsigned int width, unsigned int capacity);
void readItem(struct Performance *performance, struct Array *array, unsigned int index, void *dest);
void contract(struct Performance *performance, struct Array *array);
void writeItem(struct Performance *performance, struct Array *array, unsigned int index, void *src);
void freeArray(struct Performance *performance, struct Array *array);
void appendItem( struct Performance *performance, struct Array *array, void *src);
void insertItem( struct Performance *performance, struct Array *array, unsigned int index, void *src);
void prependItem(struct Performance *performance, struct Array *array, void *src);
void deleteItem(struct Performance *performance, struct Array *array, unsigned int index);
int findItem(struct Performance *performance, struct Array *array, int (*compar)(const void *, const void*), void *target) ;
int searchItem( struct Performance *performance, struct Array *array, int (*compar)(const void*, const void *), void *target);
struct Array {
  unsigned int width;
  unsigned int nel;
  unsigned int capacity;
  void *data;
};

struct Performance {
  unsigned int reads;
  unsigned int writes;
  unsigned int mallocs;
  unsigned int frees;
};
