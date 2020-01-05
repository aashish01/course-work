#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>

/******************************************************************************
To comple:
	mpicc -o aashish aashish.c -lcrypt
To run:
	mpirun -n 3 ./aashish
To get csv:
	mpirun -n 3 ./mr.py./ aashish|grep Time> aashish.csv

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;
char *encrypted_passwords[] = {
"$6$KB$36REBSFzq1/dui1N6SgVpJNWbH7.H09nEmDF84hZhfeauI13ShDyaMT/d1tPMVh35NJKKkQrT6OIYDS9VLrIY0",
"$6$KB$jyDvGJlpBoZ7V0LmBQMe8IRWBBOs5iptBLdOhT4LNJClRiXwfx4ul/IlCXEgzYOUjIhmBUJKNfHPVmJP3dueR1",
"$6$KB$vGjZkftJBbu1jmhrMfA8ARGIFOn6rr/AWVlkXzUU6mKwEyCyFb748wIPEdC5Q8vKFK5Fvy6uf/ZkwMvYuBoaJ/",
"$6$KB$Uz4cD9uzcYjtg9/zNnA4wdLtqlTWw42taHPdqzfJYQOmv2Ct79UJ8e11XtqdxzH3E58trHonpZFDOwYRwJPGs1"
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

//This function crack the password and display # when password is found using MPI.

void function_1(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='M'; x++){
    for(y='A'; y<='Z'; y++){
	for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
   }
  
  printf("%d solutions explored\n", count);
}

void function_2(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='N'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
	for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
   }
  
  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(int argc, char *argv[]){
  int i;
  int size, rank;
struct timespec start, finish;  
  long long int time_elapsed;
 
  clock_gettime(CLOCK_MONOTONIC, &start);//records start time of the function
  //initializing MPI
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  clock_gettime(CLOCK_MONOTONIC, &start);//records start time of the function
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  }
  else {
    if(rank ==0){
      int a;
       MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
       MPI_Send(&a, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
 
    }
    else if(rank ==1) {
      int num = rank+1;
      MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      for(int i=0;i<n_passwords;i<i++) {
       function_1(encrypted_passwords[i]);
      }
    }
    else{
       int num = rank+1;
       MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int i=0;i<n_passwords;i<i++) {
          function_2(encrypted_passwords[i]);
         }
       }
  }
  MPI_Finalize();
 
  if(rank ==0){
    clock_gettime(CLOCK_MONOTONIC, &finish);//records finish time of the function
    time_difference(&start, &finish, &time_elapsed);
    printf("Time elapsed was %lldns\n", time_elapsed);
}
return 0;
}

