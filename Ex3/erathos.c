#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
  if(argc != 2) {
    fprintf(stderr, "Error Usage : n > 0 limit");
    exit(1);
  }

  int limit = atoi(argv[1]);

  bool primes[limit];

  #pragma omp parallel shared(primes)
  {
    #pragma omp for schedule(dynamic)
    for(int i = 0; i < limit; i++) {
      primes[i] = true;
    }
  }

  int n, i;
  #pragma omp parallel shared(primes) private(n, i)
  {
    #pragma omp for schedule(dynamic)
    for(n = 2; n <= (int) sqrt(limit); n++) {
      if(primes[n]) {
        for(i = n + n; i < limit; i+= n) {
          primes[i] = false;
        }
      }
    }
  }

  int nb_primes = 0;
  for(int cpt = 2; cpt < limit; cpt++) {
    if(primes[cpt]) {
      nb_primes++;
      //
    }
  }
  printf("%d primes !\n", nb_primes);


}
