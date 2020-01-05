#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>

/***********************************************************************
Demonstrates how to crack an encrypted password using thread, Works on passwords that consist only of 2 uppercase letters and a 2 digit integer. 

To Compile:
    cc -o pthread pthread.c -lcrypt -pthread

To run and get txt file:
    ./pthread >results.txt

Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************/
int n_passwords = 4;
char *encrypted_passwords[] = {
 "$6$KB$36REBSFzq1/dui1N6SgVpJNWbH7.H09nEmDF84hZhfeauI13ShDyaMT/d1tPMVh35NJKKkQrT6OIYDS9VLrIY0",
 "$6$KB$jyDvGJlpBoZ7V0LmBQMe8IRWBBOs5iptBLdOhT4LNJClRiXwfx4ul/IlCXEgzYOUjIhmBUJKNfHPVmJP3dueR1",
 "$6$KB$vGjZkftJBbu1jmhrMfA8ARGIFOn6rr/AWVlkXzUU6mKwEyCyFb748wIPEdC5Q8vKFK5Fvy6uf/ZkwMvYuBoaJ/",
 "$6$KB$Uz4cD9uzcYjtg9/zNnA4wdLtqlTWw42taHPdqzfJYQOmv2Ct79UJ8e11XtqdxzH3E58trHonpZFDOwYRwJPGs1"
};

//Due to lack of Standard function on C following below is required

void aashish(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

//This function crack the password and display # when password is found.

void posix_thread()
{
  	int j;
	pthread_t t1, t2;

    void *kernel_function_1();
    void *kernel_function_2();

	for(j=0;j<n_passwords;j<j++) {
    pthread_create(&t1, NULL,kernel_function_1, encrypted_passwords[j]);
    pthread_create(&t2, NULL,kernel_function_2, encrypted_passwords[j]);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
 }
}

void *kernel_function_1(char *salt_and_encrypted){
  int a, d, h;     // Loop counters
  char salt[7];  
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  aashish(salt, salt_and_encrypted, 0, 6);

  for(a='A'; a<='M'; a++){
    for(d='A'; d<='Z'; d++){
      for(h=0; h<=99;  h++){
        sprintf(plain, "%c%c%02d", a,d,h);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        }
        else {
          printf(" %-8d%s %s\n", count, plain, enc);
        } 
      }
    }
  }
  printf("%d solutions explored\n", count);
}

void *kernel_function_2(char *salt_and_encrypted){
  int n, e, p;     // Loop counters
  char salt[7];  
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  aashish(salt, salt_and_encrypted, 0, 6);
  for(n='N'; n<='Z'; n++){
    for(e='A'; e<='Z'; e++){
      for(p=0; p<=99; p++){
        sprintf(plain, "%c%c%02d", n,e,p);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        }
	else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}
// Used for calculating Time

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
int main(int argc, char *argv[])
{
	struct timespec start, finish;   
  	long long int time_elapsed;
  	clock_gettime(CLOCK_MONOTONIC, &start);
    	posix_thread();
		clock_gettime(CLOCK_MONOTONIC, &finish);
	  	time_difference(&start, &finish, &time_elapsed);
	  	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,(time_elapsed/1.0e9)); 
  return 0;
}
