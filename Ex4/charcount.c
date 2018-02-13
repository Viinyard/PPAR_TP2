#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    fprintf(stderr, "Error Usage : file");
    exit(1);
  }

  int char_count[26] = {0};

  char buf[1024];
  FILE *file;
  size_t nread;

  file = fopen(argv[1], "r");
  if (file) {
    while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
      int i;
      #pragma omp parallel shared(buf, char_count) private(i)
      {
        #pragma omp for schedule(dynamic)
        for(i = 0; i < nread; i++) {
          if(buf[i] >= 'a' && buf[i] <= 'z') {
            #pragma omp critical
            {
              char_count[buf[i] - 'a'] += 1;
            }
          } else if(buf[i] >= 'A' && buf[i] <= 'Z') {
            #pragma omp critical
            {
              char_count[buf[i] - 'A'] += 1;
            }
          }
        }
      }
    }
    if (ferror(file)) {
        exit(2);
    }
    fclose(file);
  }

  for(int i = 0; i < sizeof(char_count) / sizeof(int); i++) {
    printf("%d : %c = %d\n", i, (char) i + 'a', char_count[i]);
  }

  int i;
  int voy = 0;
  int step = 1;
  int borne = sizeof(char_count) / sizeof(int);

  while(step < borne) {

    #pragma omp parallel shared(char_count, step) private (i)
    {
      #pragma omp for schedule(dynamic)
      for(i = 0; i < borne; i += step * 2) {
        char_count[i] = char_count[i] + ((i + step < borne) ? char_count[i + step] : 0);
        
        if(i + step < borne) {
  				char_count[i + step] = 0;
  			}
      }
    }
    step *= 2;
  }


  printf("Total number of character = %d\n", char_count[0]);


  for(int i = 0; i < sizeof(char_count) / sizeof(int); i++) {
    printf("%d : %c = %d\n", i, (char) i + 'a', char_count[i]);
  }

}
