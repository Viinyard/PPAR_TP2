#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char **argv) {
  srand(time(NULL));

  if(argc != 2) {
    fprintf(stderr, "Error Usage : n > 0 size of the matrix");
    exit(1);
  }

  int n = atoi(argv[1]);

  if(n <= 0) {
    fprintf(stderr, "Error Usage : n > 0 size of the matrix");
    exit(2);
  }

  int matrix_1[n][n];
  int matrix_2[n][n];
  int matrix_res[n][n];


#pragma omp parallel shared(matrix_res, matrix_1, matrix_2)
{
  #pragma omp for schedule(dynamic)
  for(int x = 0; x < n; x++) {
    for(int y = 0; y < n; y++) {
      matrix_1[x][y] = rand() % 10;
      matrix_2[x][y] = rand() % 10;
      matrix_res[x][y] = 0;
    }
  }
}

  printf("Matrice 1 :\n");
  for(int x = 0; x < n; x++) {
    for(int y = 0; y < n; y++) {
      printf("%d, ", matrix_1[x][y]);
    }
    printf("\n");
  }

  printf("Matrice 2 :\n");
  for(int x = 0; x < n; x++) {
    for(int y = 0; y < n; y++) {
      printf("%d, ", matrix_2[x][y]);
    }
    printf("\n");
  }

int x, y, i;
#pragma omp parallel shared(matrix_res, matrix_1, matrix_2) private(x, y, i)
{
  #pragma omp for schedule(dynamic)
  for( x = 0; x < n; x++) {
    for( y = 0; y < n; y++) {
      for( i = 0; i < n; i++) {
        matrix_res[x][y] += matrix_1[x][i] * matrix_2[i][y];
      }
    }
  }
}

/*
A[N][M]
B[M][P]
C[N][P]

for(i:0->N)
  for(j:0->P)
    for(k:0->M)
      C[i][j]+= A[i][k] * B[k][j]


*/

  printf("Matrice resultat :\n");
  for(int x = 0; x < n; x++) {
    for(int y = 0; y < n; y++) {
      printf("%d, ", matrix_res[x][y]);
    }
    printf("\n");
  }


}
