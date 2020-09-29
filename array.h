//Author: Sean Shaya Feigisd
//ID: 1096849
//Header file for the assignment
struct Performance *newPerformance();

struct Array {
  unsigned int width;
  unsigned int nel;
  unsigned int capacity;
  void *data;
};

struct Peformance {
  unsigned int reads;
  unsigned int writes;
  unsigned int mallocs;
  unsigned int frees;
};
