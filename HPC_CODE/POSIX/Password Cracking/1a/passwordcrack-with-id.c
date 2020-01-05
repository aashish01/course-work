#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
/***********************************************************************
To Compile:
    cc -o CrackAZ99-With-Data CrackAZ99-With-Data.c -lcrypt
To Run:
    ./CrackAZ99-With-Data > results.txt

Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************/
int n_passwords = 4;
char *encrypted_passwords[] = {
"$6$KB$36REBSFzq1/dui1N6SgVpJNWbH7.H09nEmDF84hZhfeauI13ShDyaMT/d1tPMVh35NJKKkQrT6OIYDS9VLrIY0",
"$6$KB$jyDvGJlpBoZ7V0LmBQMe8IRWBBOs5iptBLdOhT4LNJClRiXwfx4ul/IlCXEgzYOUjIhmBUJKNfHPVmJP3dueR1",
"$6$KB$vGjZkftJBbu1jmhrMfA8ARGIFOn6rr/AWVlkXzUU6mKwEyCyFb748wIPEdC5Q8vKFK5Fvy6uf/ZkwMvYuBoaJ/",
"$6$KB$Uz4cD9uzcYjtg9/zNnA4wdLtqlTWw42taHPdqzfJYQOmv2Ct79UJ8e11XtqdxzH3E58trHonpZFDOwYRwJPGs1"
};

 //Due to lack of Standard function on C following below is required.

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

//This function crack the password and display # when password is found.

void aashish(char *salt_and_encrypted){
  int p, q, r;     // Loop counters
  char salt[7]; 
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
  for(p='A'; p<='Z'; p++){
    for(q='A'; q<='Z'; q++){
      for(r=0; r<=99; r++){
        sprintf(plain, "%c%c%02d", p, q, r);
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
//Given Function is used for calculate Time.

int time_difference(struct timespec *start, struct timespec *finish, long long int *difference)
 {
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
	struct timespec start, finish;   
  	long long int time_elapsed;
  	clock_gettime(CLOCK_MONOTONIC, &start);
  	for(i=0;i<n_passwords;i<i++) 
	{
    		aashish(encrypted_passwords[i]);
  	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	  time_difference(&start, &finish, &time_elapsed);
	  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,(time_elapsed/1.0e9)); 
  return 0;
}
