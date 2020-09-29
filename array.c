#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "array.h"

struct Performance *newPerformance() {
  struct Performance temp = malloc(sizeOf(struct Performance));

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
