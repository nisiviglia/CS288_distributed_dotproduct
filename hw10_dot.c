/* 
   CS288 HW10
   By: Nick SIviglia (31360256)
*/
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define NELMS 1000000
#define MASTER 0
#define MAXPROCS 16

int dot_product();
void init_lst();
void print_lst();

int main(int argc, char **argv) {
  int i,n,vector_x[NELMS],vector_y[NELMS];
  int portion, prod, procs, tmp_prod,sidx,eidx,size;
  int pid,nprocs, rank;
  double stime,etime;
  MPI_Status status;
  MPI_Comm world;

  n = atoi(argv[1]);
  if (n > NELMS) { printf("n=%d > N=%d\n",n,NELMS); exit(1); }

  MPI_Init(&argc, &argv);
  world = MPI_COMM_WORLD;
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  
  for(i=0; i<n; i++) vector_x[i] = 1;
  for(i=0; i<n; i++) vector_y[i] = 1;

  stime = MPI_Wtime();
  portion = n/procs;
  sidx = pid * portion;
  eidx = sidx + portion;
  printf("pid=%d: sidx=%d: eidx=%d:\n", pid, sidx, eidx);
  prod = dot_product(sidx, eidx, vector_x, vector_y);
  printf("pid:%d: prod:%d\n", pid, prod);
  etime = MPI_Wtime();

  if(pid == MASTER) {
    int lst_prod[procs];
    for(i=1; i < procs; i++){
        MPI_Recv(lst_prod + i, 1, MPI_INT, i, 123, MPI_COMM_WORLD, &status);
        prod += lst_prod[i];
    }
    printf("pid=%d: final prod=%d\n",pid,prod);
    printf("pid=%d: elapsed=%f\n",pid,etime-stime);
  }
  else{
   MPI_Send(&prod, 1, MPI_INT, MASTER, 123, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}

int dot_product(int s,int e, int vector_x[], int vector_y[]){
  int i,prod=0;
  for(i=s; i < e; i++){
    prod += vector_x[i] * vector_y[i];
  }
  return prod;
}

void init_lst(int *l,int n){
  int i;
  for (i=0; i<n; i++) *l++ = i;
}
void print_lst(int s,int e, int *l){
  int i;
  for (i=s; i<e; i++) {
    printf("%x ",l[i]);
  }
  printf("\n");
}

// end of file
